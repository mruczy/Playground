// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/Data/NativeGameplayTags.h"

// Definicja kategorii logów dla GAS
DEFINE_LOG_CATEGORY_STATIC(LogHealthAttribute, Log, All);

UHealthAttributeSet::UHealthAttributeSet()
{
	MaximumHealth = 1.0f;
	CurrentHealth = 1.0f;
	HealthRegeneration = 0.0f;
	DamageOverTime = 0.f;
	DamageOverTimeHealing = 0.f;
}

void UHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	UE_LOG(LogHealthAttribute, Log, TEXT("PreAttributeBaseChange: Attribute=[%s], Incoming NewValue=[%f]"), *Attribute.GetName(), NewValue);

	if (const FVRAttributeMaxValue* MaxValue = AttributeMaxValue.Find(Attribute))
	{
		const float Max = MaxValue->MaxAttribute.IsValid() ? MaxValue->MaxAttribute.GetNumericValue(this) : MaxValue->MaxFloat.GetValueAtLevel(0);

		NewValue = FMath::Clamp(NewValue, 0.f, Max);
		return;
	}
}

void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	UE_LOG(LogHealthAttribute, Log, TEXT("Evaluated Attribute: %s, Magnitude: %f"),
		*Data.EvaluatedData.Attribute.GetName(),
		Data.EvaluatedData.Magnitude);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();
		UE_LOG(LogHealthAttribute, Log, TEXT("Processing Damage: %f"), LocalDamageDone);

		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() - LocalDamageDone;
			const float ClampedHealth = FMath::Clamp(NewHealth, 0.0f, GetMaximumHealth());
			UE_LOG(LogHealthAttribute, Log, TEXT("Applying Damage. CurrentHealth: %f, NewHealth: %f, ClampedHealth: %f, MaxHealth: %f"),
				GetCurrentHealth(), NewHealth, ClampedHealth, GetMaximumHealth());

			SetCurrentHealth(ClampedHealth);
		}
		else
		{
			UE_LOG(LogHealthAttribute, Log, TEXT("No Damage applied (Damage <= 0)"));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Store a local copy of the amount of Healing done and clear the Healing attribute.
		const float LocalHealingDone = GetHealing();
		UE_LOG(LogHealthAttribute, Log, TEXT("Processing Healing: %f"), LocalHealingDone);

		SetHealing(0.f);

		if (LocalHealingDone > 0.0f)
		{
			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() + LocalHealingDone;

			SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaximumHealth()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealthRegenerationAttribute())
	{
		UE_LOG(LogHealthAttribute, Log, TEXT("Processing HealthRegen"));
		SetHealthRegeneration(FMath::Clamp(GetHealthRegeneration(), 0.0f, GetMaximumHealth()));
		return;
	}

	UE_LOG(LogHealthAttribute, Log, TEXT("Processing None"));
}

void UHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetCurrentHealthAttribute())
	{
		CheckMaxReachedForAttribute(MaximumHealth, NativeGameplayTags::State::TAG_State_Max_Health.GetTag(), NewValue);
		return;
	}

	if (Attribute == GetDamageOverTimeAttribute())
	{
		CheckStatusTagForAttribute(NativeGameplayTags::Statuses::TAG_Debuff_DamageOverTime, NewValue, OldValue);
		return;
	}

	if (Attribute == GetMaximumHealthAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentHealthAttribute(), OldValue, NewValue);
		return;
	}
}

void UHealthAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UHealthAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaximumHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}

	if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumHealth());
		return;
	}

	if (Attribute == GetDamageOverTimeAttribute() || Attribute == GetDamageOverTimeHealingAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// To all
	DOREPLIFETIME_WITH_PARAMS_FAST(UHealthAttributeSet, CurrentHealth, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UHealthAttributeSet, MaximumHealth, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UHealthAttributeSet, DamageOverTime, Params);

	// only owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UHealthAttributeSet, HealthRegeneration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UHealthAttributeSet, DamageOverTimeHealing, Params);
}

void UHealthAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, CurrentHealth, OldValue);
}

void UHealthAttributeSet::OnRep_MaximumHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaximumHealth, OldValue);
}

void UHealthAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, HealthRegeneration, OldValue);
}

void UHealthAttributeSet::OnRep_DamageOverTime(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, DamageOverTime, OldValue);
}

void UHealthAttributeSet::OnRep_DamageOverTimeHealing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, DamageOverTimeHealing, OldValue);
}
