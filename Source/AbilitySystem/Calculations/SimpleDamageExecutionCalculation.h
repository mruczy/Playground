// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/Calculations/SimpleDamageStatics.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "SimpleDamageExecutionCalculation.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API USimpleDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
protected:

	static const FSimpleDamageStatics* DamageStatics;

public:

	USimpleDamageExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
