// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "NonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class VRPROJECT_API ANonPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	ANonPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
};
