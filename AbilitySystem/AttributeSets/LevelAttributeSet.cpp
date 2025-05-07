// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/LevelAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"


ULevelAttributeSet::ULevelAttributeSet()
{
	InitMaximumLevel(0.f);
	InitCurrentLevel(0.f);
}

void ULevelAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);

	if (Attribute != GetCurrentLevelAttribute())
	{
		return;
	}

	if (UVRAbilitySystemComponent* const AbilitySystemComponent = Cast<UVRAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
	{
		AbilitySystemComponent->ChangeLevel(NewValue);
	}
}

void ULevelAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	// to all
	DOREPLIFETIME_WITH_PARAMS_FAST(ULevelAttributeSet, CurrentLevel, Params);

	// only owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ULevelAttributeSet, MaximumLevel, Params);
}

void ULevelAttributeSet::OnRep_CurrentLevel(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULevelAttributeSet, CurrentLevel, OldValue);
}

void ULevelAttributeSet::OnRep_MaximumLevel(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULevelAttributeSet, MaximumLevel, OldValue);
}
