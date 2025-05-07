// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/MovementAttributeSet.h"
#include "Net/UnrealNetwork.h"

UMovementAttributeSet::UMovementAttributeSet()
{
	MovementSpeedMultiplier = 1.f;
	MovementAccelerationMultiplier = 1.f;
}

void UMovementAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	NewValue = 1.f;
}

void UMovementAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);

	// Make sure 
	NewValue = FMath::Max(NewValue, 0.f);
}

void UMovementAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UMovementAttributeSet, MovementSpeedMultiplier, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UMovementAttributeSet, MovementAccelerationMultiplier, Params);
}

void UMovementAttributeSet::OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, MovementSpeedMultiplier, OldValue);
}

void UMovementAttributeSet::OnRep_MovementAccelerationMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMovementAttributeSet, MovementAccelerationMultiplier, OldValue);
}
