// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "StaminaAttributeSet.generated.h"

//
// Current Stamina
// MaximumStamina
// StaminaRegeneration
UCLASS()
class VRPROJECT_API UStaminaAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	
public:

	UStaminaAttributeSet();

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_CurrentStamina)
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, CurrentStamina)

	// Holds the value for Maximum Stamina.
	UPROPERTY(BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_MaximumStamina)
	FGameplayAttributeData MaximumStamina;
	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, MaximumStamina)

	// Holds the current value for Stamina Regeneration.
	UPROPERTY(BlueprintReadOnly, Category = "Stamina Attribute Set", ReplicatedUsing = OnRep_StaminaRegeneration)
	FGameplayAttributeData StaminaRegeneration;
	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, StaminaRegeneration)

protected:

	UFUNCTION()
	virtual void OnRep_CurrentStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaximumStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_StaminaRegeneration(const FGameplayAttributeData& OldValue);
};
