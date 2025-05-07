// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "AbilitiesAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UAbilitiesAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	
public:

	UAbilitiesAttributeSet();

	static float COOLDOWN_MULTIPLIER_MAX;
	static float COOLDOWN_MULTIPLIER_MIN;

	static float COST_MULTIPLIER_MAX;
	static float COST_MULTIPLIER_MIN;

	// Attribute SEt Overrides
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Holds the current value for Resistance.
	UPROPERTY(BlueprintReadOnly, Category = "Abilities Attribute Set", ReplicatedUsing = OnRep_CooldownMultiplier)
	FGameplayAttributeData CooldownMultiplier;
	ATTRIBUTE_ACCESSORS(UAbilitiesAttributeSet, CooldownMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Abilities Attribute Set", ReplicatedUsing = OnRep_CostMultiplier)
	FGameplayAttributeData CostMultiplier;
	ATTRIBUTE_ACCESSORS(UAbilitiesAttributeSet, CostMultiplier)

	UFUNCTION(BlueprintPure, Category = "Abilities Attribute Set")
	static FVector2D GetMinMaxCooldownMultiplier() { return FVector2D(COOLDOWN_MULTIPLIER_MIN, COOLDOWN_MULTIPLIER_MAX); }

	UFUNCTION(BlueprintPure, Category = "Abilities Attribute Set")
	static FVector2D GetMinMaxCostMultiplier() { return FVector2D(COST_MULTIPLIER_MIN, COST_MULTIPLIER_MAX); }

protected:

	UFUNCTION()
	virtual void OnRep_CooldownMultiplier(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CostMultiplier(const FGameplayAttributeData& OldValue);
};
