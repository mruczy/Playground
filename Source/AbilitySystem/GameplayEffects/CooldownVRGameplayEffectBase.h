// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayEffects/VRGameplayEffect.h"
#include "CooldownVRGameplayEffectBase.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UCooldownVRGameplayEffectBase : public UVRGameplayEffect
{
	GENERATED_BODY()
	
	UCooldownVRGameplayEffectBase();

	virtual void PostLoad();
	virtual void PostInitProperties() override;
	void ConfigureEffect();
};
