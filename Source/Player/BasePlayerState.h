// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "BasePlayerState.generated.h"

class UVRAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class VRPROJECT_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	ABasePlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
