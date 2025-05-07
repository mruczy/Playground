// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VRCharacterMovementComponent.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VRPROJECT_API UVRCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	friend ABaseCharacter;

public:

	UVRCharacterMovementComponent();

	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;
	virtual void BeginPlay() override;

protected:

	float MovementSpeedMultiplier = 1.f;
};
