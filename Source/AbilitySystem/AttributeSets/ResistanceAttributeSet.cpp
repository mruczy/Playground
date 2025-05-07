// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/ResistanceAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"


float UResistanceAttributeSet::RESISTANCE_BASE = 100.f;
float UResistanceAttributeSet::RESISTANCE_MIN = -75.f;
float UResistanceAttributeSet::RESISTANCE_MAX = 200.f;

UResistanceAttributeSet::UResistanceAttributeSet()
{
	Resistance = 0.0f;
}

void UResistanceAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UResistanceAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);

	if (Attribute == GetResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, RESISTANCE_MIN, RESISTANCE_MAX);
		return;
	}
}

void UResistanceAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UResistanceAttributeSet, Resistance, Params);
}

void UResistanceAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UResistanceAttributeSet, Resistance, OldValue);
}
