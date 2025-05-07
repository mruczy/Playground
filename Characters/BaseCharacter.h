// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/AbilitySystemData.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "BaseCharacter.generated.h"

class UVRCharacterMovementComponent;

// template class
UCLASS(Abstract, NotBlueprintable)
class VRPROJECT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Movement Component")
	UVRCharacterMovementComponent* GetVRCharacterMovementComponent() const;

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "Ability System")
	UVRAbilitySystemComponent* GetVRAbilitySystemComponent() const;

	void InitializeAbilitySystem();

	// post initialization
	UFUNCTION(BlueprintNativeEvent)
	void PostInitializeAbilitySystem();

	UFUNCTION(BlueprintPure)
	const FAbilitySystemInitializationData& GetAbilitySystemInitializationData() const
	{
		return AbilitySystemInitializationData;
	}

protected:

	// Data used to initialize the AbilitySystemComponent
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System", meta = (ShowOnlyInnerProperties))
	FAbilitySystemInitializationData AbilitySystemInitializationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UVRAbilitySystemComponent> AbilitySystemComponent;

	void MovementSpeedMultiplierChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
};
