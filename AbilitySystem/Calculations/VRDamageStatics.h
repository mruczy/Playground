#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "AbilitySystem/AttributeSets/ResistanceAttributeSet.h"

struct FVRDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaximumHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ReceivedDamageOverTime);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageOverTime);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resistance);

	FVRDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, CurrentHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, MaximumHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, Damage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, ReceivedDamageOverTime, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, DamageOverTime, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UResistanceAttributeSet, Resistance, Target, false);
	}

	static const FVRDamageStatics& DamageStatics()
	{
		static FVRDamageStatics DamageStatics;
		return DamageStatics;
	}
};