// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ResistanceAttributeSet.generated.h"

//
//
// Resistance 
UCLASS()
class VRPROJECT_API UResistanceAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()

public:

	UResistanceAttributeSet();

	static float RESISTANCE_BASE;
	static float RESISTANCE_MAX;
	static float RESISTANCE_MIN;

	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Current value of Resistance
	UPROPERTY(BlueprintReadOnly, Category = "Resistance Attribute Set", ReplicatedUsing = OnRep_Resistance)
	FGameplayAttributeData Resistance;
	ATTRIBUTE_ACCESSORS(UResistanceAttributeSet, Resistance)

	UFUNCTION(BlueprintPure, Category = "Resistance attribute Set")
	static float GetBaseResistance() { return RESISTANCE_BASE; }

	UFUNCTION(BlueprintPure, Category = "Resistance attribute Set")
	static FVector2D GetMinMaxResistance() { return FVector2D(RESISTANCE_MIN, RESISTANCE_MAX); }

protected:

	UFUNCTION()
	virtual void OnRep_Resistance(const FGameplayAttributeData& OldValue);
};