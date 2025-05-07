// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Calculations/SimpleDamageExecutionCalculation.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "AbilitySystem/FunctionLibrary/VRAbilitySystemBlueprintLibrary.h"

const FSimpleDamageStatics* USimpleDamageExecutionCalculation::DamageStatics = nullptr;

USimpleDamageExecutionCalculation::USimpleDamageExecutionCalculation()
{
	if (!DamageStatics)
	{
		DamageStatics = &FSimpleDamageStatics::DamageStatics();
	}

	RelevantAttributesToCapture.Add(DamageStatics->CurrentHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics->MaximumHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics->DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics->ResistanceDef);
}

void USimpleDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	if (!DamageStatics)
	{
		DamageStatics = &FSimpleDamageStatics::DamageStatics();
	}

	if (!DamageStatics)
	{
		return;
	}

	UVRAbilitySystemComponent* const TargetAbilitySystemComponent = Cast<UVRAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());

	if (!TargetAbilitySystemComponent)
	{
		return;
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters{};
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float DamageRaw = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->DamageDef, EvaluationParameters, DamageRaw);
	DamageRaw = FMath::Max(DamageRaw, 0.f);

	if (DamageRaw == 0.f)
	{
		return;
	}

	float CurrentResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics->ResistanceDef, EvaluationParameters, CurrentResistance);
	CurrentResistance = FMath::Clamp(CurrentResistance, UResistanceAttributeSet::RESISTANCE_MIN, UResistanceAttributeSet::RESISTANCE_MAX);

	const float ResistanceDamageMultiplier = UVRAbilitySystemBlueprintLibrary::CalculateEffectiveResistance(CurrentResistance);

	const float FinalDamage = DamageRaw * ResistanceDamageMultiplier;

	if (FinalDamage <= 0.f || FMath::IsNearlyZero(DamageRaw, 0.01f))
	{
		return;
	}

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics->CurrentHealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
