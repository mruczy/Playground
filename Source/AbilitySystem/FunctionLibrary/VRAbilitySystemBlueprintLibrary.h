// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "VRAbilitySystemBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UVRAbilitySystemBlueprintLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()
	
public:

	// Attempts to find the value of the Attribute supplied.
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	static float GetAttributeValueFromAbilitySystem(const UAbilitySystemComponent* const AbilitySystemComponent, const FGameplayAttribute Attribute, const EAttributeSearchType SearchType);

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	static float CalculateEffectiveResistance(const float CurrentArmor);

	UFUNCTION(BlueprintCallable, Category = "Ability System", meta = (AutoCreateRefTerm = "ContextString"))
	static void SetTargetOnGameplayEffectContext(FGameplayEffectContextHandle& ContextHandle, const AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category = "Ability System", meta = (AutoCreateRefTerm = "ContextString"))
	static void SetTargetOnGameplayEffectContextFromSpec(FGameplayEffectSpec& EffectSpec, const AActor* TargetActor);

private:

	static void GetAttributeValue(const UAbilitySystemComponent* const AbilitySystemComponent, const FGameplayAttribute& Attribute, const EAttributeSearchType SearchType, OUT float& ReturnValue);
};
