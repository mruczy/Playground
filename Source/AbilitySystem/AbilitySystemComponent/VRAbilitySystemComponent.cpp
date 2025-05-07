// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/LevelAttributeSet.h"
#include "AbilitySystem/Data/NativeGameplayTags.h"
#include "AbilitySystem/FunctionLibrary/VRAbilitySystemBlueprintLibrary.h"
#include "Characters/BaseCharacter.h"

UVRAbilitySystemComponent::UVRAbilitySystemComponent()
{
	// Sets the Ability System Component to use "Mixed" replication mode. This will replicate minimal Gameplay Effects to Simulated Proxies and full info to everyone else.
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;

	SetIsReplicatedByDefault(true);
}

void UVRAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

void UVRAbilitySystemComponent::InitializeAbilitySystemData(const FAbilitySystemInitializationData& InitializationData, AActor* InOwningActor, AActor* InAvatarActor)
{
	if (AbilitySystemDataInitialized)
	{
		return;
	}

	AbilitySystemDataInitialized = true;

	// Set the Owning Actor and Avatar Actor. (Used throughout the Gameplay Ability System to get references etc.)
	InitAbilityActorInfo(InOwningActor, InAvatarActor);

	// Apply the Gameplay Tag container as loose Gameplay Tags. (These are not replicated by default and should be applied on both server and client respectively.)
	if (!InitializationData.GameplayTags.IsEmpty())
	{
		AddLooseGameplayTags(InitializationData.GameplayTags);
	}

	/** This is our entry point for other component to react to gameplay effect added and removed.
	 * Changes to the gameplay effects are handled differently (using their own delegate sets)
	 * -> FActiveGameplayEffectEvents* EventSet = ASC->GetActiveEffectEventSet(Handle);
	 * (See EffectWidgetControllerBase::InitializeController_Implementation)
	 */
	OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &UVRAbilitySystemComponent::OnGameplayEffectAddedCallback);
	ActiveGameplayEffects.OnActiveGameplayEffectRemovedDelegate.AddUObject(this, &UVRAbilitySystemComponent::OnGameplayEffectRemovedCallback);

	// Check to see if we have authority. (Attribute Sets / Attribute Base Values / Gameplay Abilities / Gameplay Effects should only be added -or- set on authority and will be replicated to the client automatically.)
	if (!GetOwnerActor()->HasAuthority())
	{
		return;
	}

	// Grant Attribute Sets if the array isn't empty.
	if (!InitializationData.AttributeSets.IsEmpty())
	{
		for (const TSubclassOf<UAttributeSet>& AttributeSetClass : InitializationData.AttributeSets)
		{
			GetOrCreateAttributeSet(AttributeSetClass);
		}
	}

	// Set base attribute values if the map isn't empty.
	if (!InitializationData.AttributeBaseValues.IsEmpty())
	{
		for (const TTuple<FGameplayAttribute, FVRAttributeInitializer>& AttributeBaseValue : InitializationData.AttributeBaseValues)
		{
			if (HasAttributeSetForAttribute(AttributeBaseValue.Key))
			{
				SetNumericAttributeBase(AttributeBaseValue.Key, AttributeBaseValue.Value.Value.GetValueAtLevel(0.f));
			}
		}
	}

	// Grant Gameplay Abilities if the array isn't empty.
	if (!InitializationData.GameplayAbilities.IsEmpty())
	{
		for (const TSubclassOf<UGameplayAbility>& GameplayAbility : InitializationData.GameplayAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbility, 0, INDEX_NONE, this);

			GiveAbility(AbilitySpec);
		}
	}

	// Apply Gameplay Effects if the array isn't empty.
	if (!InitializationData.GameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : InitializationData.GameplayEffects)
		{
			if (!IsValid(GameplayEffect))
			{
				continue;
			}

			FGameplayEffectContextHandle EffectContexHandle = MakeEffectContext();
			EffectContexHandle.AddSourceObject(this);

			if (FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingSpec(GameplayEffect, 1, EffectContexHandle); GameplayEffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), this);
			}
		}
	}

	ChangeLevel(GetNumericAttribute(ULevelAttributeSet::GetCurrentLevelAttribute()), true);
}

const UAttributeSet* UVRAbilitySystemComponent::GetOrCreateAttributeSet(const TSubclassOf<UAttributeSet>& AttributeSet)
{
	return GetOrCreateAttributeSubobject(AttributeSet);
}

void UVRAbilitySystemComponent::ChangeLevel_Implementation(const float DesiredLevel, const bool bInitialization /* = false */)
{
	// Get maximum and current level and check if desired new level is ok
	const float MaximumLevel = GetNumericAttribute(ULevelAttributeSet::GetMaximumLevelAttribute());
	const float CurrentLevel = GetNumericAttribute(ULevelAttributeSet::GetCurrentLevelAttribute());

	const float NewLevel = FMath::Clamp(DesiredLevel, 0.f, MaximumLevel);

	if (FMath::IsNearlyEqual(NewLevel, CurrentLevel, 0.1f))
	{
		return;
	}

	// Set new level
	SetNumericAttributeBase(ULevelAttributeSet::GetCurrentLevelAttribute(), NewLevel);

	// Update all Attributes that should be leveled up
	if (const ABaseCharacter* const BaseCharacter = GetBaseCharacterAvatar())
	{
		for (const TTuple<FGameplayAttribute, FVRAttributeInitializer>& AttributeBaseValue : BaseCharacter->GetAbilitySystemInitializationData().AttributeBaseValues)
		{
			// Dont touch level itself
			if (AttributeBaseValue.Key.GetAttributeSetClass() == ULevelAttributeSet::StaticClass())
			{
				continue;
			}

			// Dont touch current attribute if they are marked to not level up
			if (!bInitialization && NoLevelAttribute.Contains(AttributeBaseValue.Key))
			{
				continue;
			}

			// Update the Attribute with new level
			if (HasAttributeSetForAttribute(AttributeBaseValue.Key))
			{
				const float NewValue = AttributeBaseValue.Value.Value.GetValueAtLevel(NewLevel);
				SetNumericAttributeBase(AttributeBaseValue.Key, NewValue);
			}
		}
	}

	// Update all GameplayEffects
	const int32 NewLevelInt = FMath::Floor(NewLevel);

	// Dont change the level of any GameplayEffects with no level tag
	FGameplayEffectQuery Query{};
	Query.EffectTagQuery.MakeQuery_MatchNoTags(NativeGameplayTags::TAG_Effect_NoLevel.GetTag().GetSingleTagContainer());

	SetActiveGameplayEffectLevelUsingQuery(Query, NewLevelInt);

	// Update all Abilities
	for (FGameplayAbilitySpec& ActivatableAbility : GetActivatableAbilities())
	{
		if (!ActivatableAbility.Level == NewLevelInt)
		{
			continue;
		}
		ActivatableAbility.Level = NewLevelInt;
		MarkAbilitySpecDirty(ActivatableAbility, false);
	}
}

ABaseCharacter* UVRAbilitySystemComponent::GetBaseCharacterAvatar() const
{
	return Cast<ABaseCharacter>(GetAvatarActor_Direct());
}

FActiveGameplayEffectHandle UVRAbilitySystemComponent::SetActiveGameplayEffectInhibit(FActiveGameplayEffectHandle&& ActiveGEHandle, bool bInhibit, bool bInvokeGameplayCueEvents)
{
	FActiveGameplayEffectHandle NewHandle = Super::SetActiveGameplayEffectInhibit(MoveTemp(ActiveGEHandle), bInhibit, bInvokeGameplayCueEvents);

	if (NewHandle.IsValid() && bInhibit)
	{
		FActiveGameplayEffect* const ActiveGameplayEffect = ActiveGameplayEffects.GetActiveGameplayEffect(ActiveGEHandle);
		if (!ActiveGameplayEffect)
		{
			return NewHandle;
		}
		OnGameplayEffectAddedCallback(this, ActiveGameplayEffect->Spec, NewHandle);
	}

	return NewHandle;
}

void UVRAbilitySystemComponent::OnGameplayEffectAddedCallback(UAbilitySystemComponent* const ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	// Getting the Effect information, if they exist (which should work) and broadast them as "Added"
	if (const FActiveGameplayEffect* ActiveEffect = GetActiveGameplayEffect(Handle))
	{
		OnVRGameplayEffectEventDelegate.Broadcast(EVREffectEventType::Added, *ActiveEffect);
	}
}

void UVRAbilitySystemComponent::OnGameplayEffectRemovedCallback(const FActiveGameplayEffect& ActiveGameplayEffect)
{
	// Broadcast the removal of this effect with the CustomEffectEventType::Removed.
	OnVRGameplayEffectEventDelegate.Broadcast(EVREffectEventType::Removed, ActiveGameplayEffect);
}

float UVRAbilitySystemComponent::GetFilteredAttribute(const FGameplayAttribute Attribute, const FGameplayTagRequirements SourceTags, const FGameplayTagContainer TargetTags)
{
	return GetFilteredAttributeValue(Attribute, SourceTags, TargetTags);
}

void UVRAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);
}

void UVRAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
	}
}

FActiveGameplayEffectHandle UVRAbilitySystemComponent::ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpec& GameplayEffect, UAbilitySystemComponent* Target, FPredictionKey PredictionKey)
{
	if (Target->GetOwner())
	{
		UVRAbilitySystemBlueprintLibrary::SetTargetOnGameplayEffectContextFromSpec(const_cast<FGameplayEffectSpec&>(GameplayEffect), Target->GetOwner());
	}

	return Super::ApplyGameplayEffectSpecToTarget(GameplayEffect, Target, PredictionKey);
}
