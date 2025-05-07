// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"

UAbilitySystemComponent* ABasePlayerController::GetAbilitySystemComponent() const
{
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner.Get());
}
