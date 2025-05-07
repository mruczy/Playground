// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/StaminaAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/Data/NativeGameplayTags.h"

UStaminaAttributeSet::UStaminaAttributeSet()
{
	MaximumStamina = 1.0f;
	CurrentStamina = 1.0f;
	StaminaRegeneration = 0.0f;
}

void UStaminaAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);

	if (Attribute == GetCurrentStaminaAttribute())
	{
		CheckMaxReachedForAttribute(MaximumStamina, NativeGameplayTags::State::TAG_State_Max_Stamina.GetTag(), NewValue);
		return;
	}

	if (Attribute == GetMaximumStaminaAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentStaminaAttribute(), OldValue, NewValue);
		return;
	}
}

void UStaminaAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);

	if (Attribute == GetCurrentStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumStamina());
		return;
	}

	if (Attribute == GetMaximumStaminaAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
		return;
	}
}

void UStaminaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UStaminaAttributeSet, CurrentStamina, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UStaminaAttributeSet, MaximumStamina, Params);

	// only owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UStaminaAttributeSet, StaminaRegeneration, Params);
}

void UStaminaAttributeSet::OnRep_CurrentStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaAttributeSet, CurrentStamina, OldValue);
}

void UStaminaAttributeSet::OnRep_MaximumStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaAttributeSet, MaximumStamina, OldValue);
}

void UStaminaAttributeSet::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaAttributeSet, StaminaRegeneration, OldValue);
}
