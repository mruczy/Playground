// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePlayerState.h"
#include "AbilitySystemBlueprintLibrary.h"

ABasePlayerState::ABasePlayerState()
{
}

UAbilitySystemComponent* ABasePlayerState::GetAbilitySystemComponent() const
{
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
}
