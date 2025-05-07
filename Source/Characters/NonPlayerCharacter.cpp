// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NonPlayerCharacter.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"




ANonPlayerCharacter::ANonPlayerCharacter(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsRunningDedicatedServer())
	{
		InitializeAbilitySystem();
	}
}
