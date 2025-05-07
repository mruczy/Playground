// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/PlayerState.h"
#include "Characters/VRCharacterMovementComponent.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/MovementAttributeSet.h"


ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	//: Super(ObjectInitializer.SetDefaultSubobjectClass<UVRCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// If the NetUpdateFrequency is too low, there will be a delay on Ability activation / Effect application on the client.
	SetNetUpdateFrequency(100.0f);

	// Create the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<UVRAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UVRCharacterMovementComponent* ABaseCharacter::GetVRCharacterMovementComponent() const
{
	return Cast<UVRCharacterMovementComponent>(GetCharacterMovement());
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UVRAbilitySystemComponent* ABaseCharacter::GetVRAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ABaseCharacter::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Call the function on "Custom Ability System Component" to set up references and Init data. (Client)
	AbilitySystemComponent->InitializeAbilitySystemData(AbilitySystemInitializationData, this, this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMovementAttributeSet::GetMovementSpeedMultiplierAttribute()).AddUObject(this, &ThisClass::MovementSpeedMultiplierChanged);

	PostInitializeAbilitySystem();
}

void ABaseCharacter::PostInitializeAbilitySystem_Implementation()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
}

void ABaseCharacter::MovementSpeedMultiplierChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	GetVRCharacterMovementComponent()->MovementSpeedMultiplier = OnAttributeChangeData.NewValue;
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeAbilitySystem();
}

void ABaseCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	InitializeAbilitySystem();
}



