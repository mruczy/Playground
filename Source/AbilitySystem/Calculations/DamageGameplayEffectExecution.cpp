// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Calculations/DamageGameplayEffectExecution.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "AbilitySystem/Data/NativeGameplayTags.h"
#include "AbilitySystem/FunctionLibrary/VRAbilitySystemBlueprintLibrary.h"
#include "Characters/BaseCharacter.h"

// Definicja kategorii logów dla GAS
DEFINE_LOG_CATEGORY_STATIC(LogDamageExecution, Log, All);

const FVRDamageStatics* UDamageGameplayEffectExecution::DamageStatics = nullptr;

UDamageGameplayEffectExecution::UDamageGameplayEffectExecution()
{
	if (!DamageStatics)
	{
		UE_LOG(LogDamageExecution, Error, TEXT("Failed to initialize DamageStatics in constructor."));
		DamageStatics = &FVRDamageStatics::DamageStatics();
	}

	RelevantAttributesToCapture.Add(DamageStatics->CurrentHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics->MaximumHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics->ReceivedDamageOverTimeDef);
	RelevantAttributesToCapture.Add(DamageStatics->DamageOverTimeDef);
	RelevantAttributesToCapture.Add(DamageStatics->DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics->ResistanceDef);
}

void UDamageGameplayEffectExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UE_LOG(LogDamageExecution, Log, TEXT("Starting Execute_Implementation for Gameplay Effect."));

	if (!DamageStatics)
	{
		UE_LOG(LogDamageExecution, Error, TEXT("DamageStatics is null in Execute_Implementation."));
		DamageStatics = &FVRDamageStatics::DamageStatics();
	}

	if (!DamageStatics)
	{
		return;
	}

	UVRAbilitySystemComponent* const TargetAbilitySystemComponent = Cast<UVRAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());

	if (!TargetAbilitySystemComponent)
	{
		UE_LOG(LogDamageExecution, Error, TEXT("TargetAbilitySystemComponent is null or invalid."));
		return;
	}
	UE_LOG(LogDamageExecution, Log, TEXT("TargetAbilitySystemComponent found: %s"), *TargetAbilitySystemComponent->GetName());

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters{};
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	UE_LOG(LogDamageExecution, Verbose, TEXT("Source Tags: %s"), EvaluationParameters.SourceTags ? *EvaluationParameters.SourceTags->ToString() : TEXT("None"));
	UE_LOG(LogDamageExecution, Verbose, TEXT("Target Tags: %s"), EvaluationParameters.TargetTags ? *EvaluationParameters.TargetTags->ToString() : TEXT("None"));

	// Apply Status Damage
	UE_LOG(LogDamageExecution, Log, TEXT("Attempting to apply Direct Damage."));
	CalculateDamageAndApply(
		*TargetAbilitySystemComponent,
		Spec,
		-1.f, // Decrease Current Health
		OutExecutionOutput,
		ExecutionParams,
		EvaluationParameters,
		DamageStatics->DamageDef,
		DamageStatics->CurrentHealthProperty,
		NativeGameplayTags::BasicAttack::TAG_DamageType_DirectDamage.GetTag(),
		NativeGameplayTags::BasicAttack::TAG_Resistance_Modifier_DirectDamage.GetTag(),
		NativeGameplayTags::BasicAttack::TAG_Resistance_Ignore_DirectDamage.GetTag(),
		NativeGameplayTags::Statuses::TAG_Immunity_DamageOverTime.GetTag()
	);

	UE_LOG(LogDamageExecution, Log, TEXT("Attempting to apply Damage Over Time (Bleed)."));
	CalculateDamageAndApply(
		*TargetAbilitySystemComponent,
		Spec,
		1.f, // Increase Bleed
		OutExecutionOutput,
		ExecutionParams,
		EvaluationParameters,
		DamageStatics->ReceivedDamageOverTimeDef,
		DamageStatics->DamageOverTimeProperty,
		NativeGameplayTags::Statuses::TAG_DamageType_DamageOverTime.GetTag(),
		NativeGameplayTags::Statuses::TAG_Resistance_Modifier_DamageOverTime.GetTag(),
		NativeGameplayTags::Statuses::TAG_Resistance_Ignore_DamageOverTime.GetTag(),
		NativeGameplayTags::Statuses::TAG_Immunity_DamageOverTime.GetTag()
	);

	UE_LOG(LogDamageExecution, Log, TEXT("Execute_Implementation completed."));
}

void UDamageGameplayEffectExecution::CalculateDamageAndApply(
	const UVRAbilitySystemComponent& TargetAbilitySystemComponent,
	const FGameplayEffectSpec& Spec, 
	float DamageSign, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput, 
	const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	const FAggregatorEvaluateParameters& EvaluationParameters, 
	const FGameplayEffectAttributeCaptureDefinition& AttributeDef, 
	FProperty* OutputProperty, 
	const FGameplayTag& DamageType, 
	const FGameplayTag& ResistanceTag, 
	const FGameplayTag& IgnoreResistanceTag, 
	const FGameplayTag& ImmunityTag)
{
	UE_LOG(LogDamageExecution, Log, TEXT("CalculateDamageAndApply called for DamageType: %s"), *DamageType.ToString());

	float DamageRaw = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributeDef, EvaluationParameters, DamageRaw);
	DamageRaw = FMath::Max(DamageRaw, 0.f);

	UE_LOG(LogDamageExecution, Log, TEXT("Raw Damage calculated: %f"), DamageRaw);

	if (DamageRaw == 0.f)
	{
		UE_LOG(LogDamageExecution, Warning, TEXT("DamageRaw is 0 for DamageType: %s. Skipping application."), *DamageType.ToString());
		return;
	}

	if (ImmunityTag.IsValid() && TargetAbilitySystemComponent.HasMatchingGameplayTag(ImmunityTag))
	{
		UE_LOG(LogDamageExecution, Warning, TEXT("Target is immune to DamageType: %s due to ImmunityTag: %s"), *DamageType.ToString(), *ImmunityTag.ToString());
		return;
	}

	float ResistanceDamageMultiplier = 1.f;

	const bool bIgnoreResistance = IgnoreResistanceTag.IsValid() && EvaluationParameters.SourceTags->HasAny(IgnoreResistanceTag.GetSingleTagContainer());
	if (!bIgnoreResistance)
	{
		FAggregatorEvaluateParameters EvaluationParameterWithDamageType{};
		bool bUseDynamicDamageTypeTag = false;

		if (DamageType.IsValid())
		{
			EvaluationParameterWithDamageType = EvaluationParameters;

			// Add damage tags
			FGameplayTagContainer SourceEvaluationWithDamageType = *EvaluationParameters.SourceTags;
			SourceEvaluationWithDamageType.AddTag(DamageType);
			EvaluationParameterWithDamageType.SourceTags = &SourceEvaluationWithDamageType;

			bUseDynamicDamageTypeTag = true;
			UE_LOG(LogDamageExecution, Verbose, TEXT("Using dynamic DamageType tag: %s"), *DamageType.ToString());
		}

		float CurrentResistance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			DamageStatics->ResistanceDef,
			bUseDynamicDamageTypeTag ? EvaluationParameterWithDamageType : EvaluationParameters,
			CurrentResistance
		);
		CurrentResistance = FMath::Clamp(CurrentResistance, UResistanceAttributeSet::RESISTANCE_MIN, UResistanceAttributeSet::RESISTANCE_MAX);

		UE_LOG(LogDamageExecution, Log, TEXT("Current Resistance: %f"), CurrentResistance);

		ResistanceDamageMultiplier = UVRAbilitySystemBlueprintLibrary::CalculateEffectiveResistance(CurrentResistance);
		UE_LOG(LogDamageExecution, Log, TEXT("Resistance Damage Multiplier: %f"), ResistanceDamageMultiplier);
	}

	// Calculate final damage
	const float FinalDamage = DamageRaw * ResistanceDamageMultiplier;
	UE_LOG(LogDamageExecution, Log, TEXT("Final Damage calculated: %f (Sign: %f)"), FinalDamage, DamageSign);
	
	if (FinalDamage <= 0.f || FMath::IsNearlyZero(DamageRaw, 0.01f))
	{
		UE_LOG(LogDamageExecution, Warning, TEXT("Final Damage is 0 or near-zero for DamageType: %s. Skipping application."), *DamageType.ToString());
		return;
	}

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			OutputProperty,
			EGameplayModOp::Additive,
			FinalDamage * FMath::Sign(DamageSign)
			)
	);
	UE_LOG(LogDamageExecution, Log, TEXT("Applied Gameplay Effect modifier: Property=%s, Value=%f"), *OutputProperty->GetName(), FinalDamage * FMath::Sign(DamageSign));
}
