// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/FunctionLibrary/VRAbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/ResistanceAttributeSet.h"
#include "AbilitySystem/Data/AbilitySystemData.h"

float UVRAbilitySystemBlueprintLibrary::GetAttributeValueFromAbilitySystem(const UAbilitySystemComponent* const AbilitySystemComponent, const FGameplayAttribute Attribute, const EAttributeSearchType SearchType)
{
	float ReturnValue = -1.0f;

	GetAttributeValue(AbilitySystemComponent, Attribute, SearchType, ReturnValue);

	return ReturnValue;
}

float UVRAbilitySystemBlueprintLibrary::CalculateEffectiveResistance(const float CurrentArmor)
{
	const float CurrentArmomrTmp = FMath::Clamp(CurrentArmor, UResistanceAttributeSet::RESISTANCE_MIN, UResistanceAttributeSet::RESISTANCE_MAX);

	const float ArmorDenominator = UResistanceAttributeSet::RESISTANCE_BASE + CurrentArmomrTmp;
	return UResistanceAttributeSet::RESISTANCE_BASE > 0.f && ArmorDenominator > 0.f ? UResistanceAttributeSet::RESISTANCE_BASE / ArmorDenominator : 1.f;
}

void UVRAbilitySystemBlueprintLibrary::SetTargetOnGameplayEffectContext(FGameplayEffectContextHandle& ContextHandle, const AActor* TargetActor)
{
	if (FVRGameplayEffectContext* const EffectContext = static_cast<FVRGameplayEffectContext*>(ContextHandle.Get()))
	{
		EffectContext->SetTargetActor(TargetActor);
	}
}

void UVRAbilitySystemBlueprintLibrary::SetTargetOnGameplayEffectContextFromSpec(FGameplayEffectSpec& EffectSpec, const AActor* TargetActor)
{
	SetTargetOnGameplayEffectContext(const_cast<FGameplayEffectContextHandle&>(EffectSpec.GetEffectContext()), TargetActor);
}

void UVRAbilitySystemBlueprintLibrary::GetAttributeValue(const UAbilitySystemComponent* const AbilitySystemComponent, const FGameplayAttribute& Attribute, const EAttributeSearchType SearchType, OUT float& ReturnValue)
{
	ReturnValue = -1.0f;

	if (!AbilitySystemComponent || !AbilitySystemComponent->HasAttributeSetForAttribute(Attribute))
	{
		return;
	}

	switch (SearchType)
	{
	case EAttributeSearchType::FinalValue:
	{
		ReturnValue = AbilitySystemComponent->GetNumericAttribute(Attribute);

		return;
	}

	case EAttributeSearchType::BaseValue:
	{
		ReturnValue = AbilitySystemComponent->GetNumericAttributeBase(Attribute);

		return;
	}

	case EAttributeSearchType::BonusValue:
	{
		ReturnValue = AbilitySystemComponent->GetNumericAttribute(Attribute) - AbilitySystemComponent->
			GetNumericAttributeBase(Attribute);
	}
	}
}
