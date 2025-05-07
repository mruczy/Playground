// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGameplayTag;
// Base class for AttributeSet with Macros and helper functions
UCLASS()
class VRPROJECT_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
protected:
	void AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, const float OldMaxValue, const float NewMaxValue) const;

	void CheckMaxReachedForAttribute(const FGameplayAttributeData& MaxAttribute, const FGameplayTag& MaxTag, const float& NewValue) const;
	void CheckStatusTagForAttribute(const FGameplayTag& StatusTag, const float& NewValue, const float& OldValue) const;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const {};
};
