// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "MovementAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class VRPROJECT_API UMovementAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	
public:

	UMovementAttributeSet();

	// Attribute Set Overrides.
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Movement Attribute Set", ReplicatedUsing = OnRep_MovementSpeedMultiplier)
	FGameplayAttributeData MovementSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MovementSpeedMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Movement Attribute Set", ReplicatedUsing = OnRep_MovementAccelerationMultiplier)
	FGameplayAttributeData MovementAccelerationMultiplier;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MovementAccelerationMultiplier)

protected:

	UFUNCTION()
	virtual void OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MovementAccelerationMultiplier(const FGameplayAttributeData& OldValue);
};
