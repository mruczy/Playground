// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/VRCharacterMovementComponent.h"

UVRCharacterMovementComponent::UVRCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

float UVRCharacterMovementComponent::GetMaxSpeed() const
{
	return Super::GetMaxSpeed() * MovementSpeedMultiplier;
}

float UVRCharacterMovementComponent::GetMaxAcceleration() const
{
	return Super::GetMaxAcceleration();
}

void UVRCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}
