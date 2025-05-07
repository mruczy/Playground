#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "AbilitySystem/AttributeSets/ResistanceAttributeSet.h"

struct FSimpleDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaximumHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resistance);

	FSimpleDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, CurrentHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, MaximumHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, Damage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UResistanceAttributeSet, Resistance, Target, false);
	}

	static const FSimpleDamageStatics& DamageStatics()
	{
		static FSimpleDamageStatics DamageStatics;
		return DamageStatics;
	}
};