// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

// Player Character is the base class to use for any Client controlled Characters.
// This class gets its Ability System Component from the Player State Base class.
UCLASS(Blueprintable)
class VRPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	void HandleMovementInput(const float InputX, const float InputY);

	// Acknowledge possesion of this Character
	virtual void PossessedBy(AController* NewController) override;
};
