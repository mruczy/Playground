// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/Calculations/VRDamageStatics.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "DamageGameplayEffectExecution.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UDamageGameplayEffectExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
protected:

	static const FVRDamageStatics* DamageStatics;

public:

	UDamageGameplayEffectExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	/**
	 * Convenience Method to calculate status damage. Can be reused for any new status.
	 * @param TargetASC Target Ability System Component
	 * @param Spec
	 * @param DamageSign
	 * @param OutExecutionOutput
	 * @param ExecutionParams
	 * @param EvaluationParameters
	 * @param AttributeDef Status damage received attribute
	 * @param OutputProperty Status to update
	 * @param DamageType
	 * @param ResistanceTag
	 * @param IgnoreResistanceTag
	 * @param ImmunityTag Immunity tag, if any (if invalid -> no immunity). Only checks GE's asset tags (for now).
	 * @param DamageMultiplier Any damage multiplier you want to apply. Could be armor, or anything else.
	 */
	static void CalculateDamageAndApply(
		const UVRAbilitySystemComponent& TargetAbilitySystemComponent,
		const FGameplayEffectSpec& Spec,
		float DamageSign,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput,
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		const FAggregatorEvaluateParameters& EvaluationParameters,
		const FGameplayEffectAttributeCaptureDefinition& AttributeDef,
		FProperty* OutputProperty,
		const FGameplayTag& DamageType = FGameplayTag::EmptyTag,
		const FGameplayTag& ResistanceTag = FGameplayTag::EmptyTag,
		const FGameplayTag& IgnoreResistanceTag = FGameplayTag::EmptyTag,
		const FGameplayTag& ImmunityTag = FGameplayTag::EmptyTag
	);
};
