// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API ABasePlayerController : public APlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
