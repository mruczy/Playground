// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"

void UBaseAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, const float OldMaxValue, const float NewMaxValue) const
{
	UAbilitySystemComponent* const AbilitySystemComponent = GetOwningAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (OldMaxValue <= 0.f || FMath::IsNearlyEqual(OldMaxValue, NewMaxValue, 0.f))
	{
		return;
	}

	// Change current value to maintain current / max value percentage
	AbilitySystemComponent->SetNumericAttributeBase(AffectedAttribute, AbilitySystemComponent->GetNumericAttributeBase(AffectedAttribute) * NewMaxValue / OldMaxValue);
}

void UBaseAttributeSet::CheckMaxReachedForAttribute(const FGameplayAttributeData& MaxAttribute, const FGameplayTag& MaxTag, const float& NewValue) const
{
	UAbilitySystemComponent* const AbilitySystemComponent = GetOwningAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	const float Max = MaxAttribute.GetCurrentValue();
	const bool bHasTag = AbilitySystemComponent->HasMatchingGameplayTag(MaxTag);

	int32 Count = -1;
	if (NewValue < Max && !bHasTag)
	{
		Count = 0;
	} 
	else if (NewValue >= Max && !bHasTag)
	{
			Count = 1;
	}

	if (Count >= 0)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(MaxTag, Count);
		AbilitySystemComponent->SetReplicatedLooseGameplayTagCount(MaxTag, Count);
	}
}

void UBaseAttributeSet::CheckStatusTagForAttribute(const FGameplayTag& StatusTag, const float& NewValue, const float& OldValue) const
{
	UAbilitySystemComponent* const AbilitySystemComponent = GetOwningAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	int32 Count = -1;
	if (NewValue <= 0.f && OldValue > 0.f)
	{
		Count = 0;
	} 
	else if (NewValue > 0.f && OldValue <= 0.f)
	{
		Count = 1;
	}

	if (Count >= 0)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(StatusTag, Count);
		AbilitySystemComponent->SetReplicatedLooseGameplayTagCount(StatusTag, Count);
	}
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttributes(Attribute, NewValue);
}

void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttributes(Attribute, NewValue);
}
