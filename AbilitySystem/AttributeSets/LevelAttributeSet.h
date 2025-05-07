// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LevelAttributeSet.generated.h"

//
//
// CurrentLevel
// MaximumLevel
UCLASS()
class VRPROJECT_API ULevelAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	
public:

	ULevelAttributeSet();

	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Level Attribute Set", Replicated = OnRep_CurrentLevel)
	FGameplayAttributeData CurrentLevel;
	ATTRIBUTE_ACCESSORS(ULevelAttributeSet, CurrentLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Level Attribute Set", Replicated = OnRep_MaximumLevel)
	FGameplayAttributeData MaximumLevel;
	ATTRIBUTE_ACCESSORS(ULevelAttributeSet, MaximumLevel)

protected:

	UFUNCTION()
	virtual void OnRep_CurrentLevel(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaximumLevel(const FGameplayAttributeData& OldValue);
};
