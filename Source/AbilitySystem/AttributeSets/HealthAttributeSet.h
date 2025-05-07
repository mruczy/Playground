// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSets/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilitySystemData.h"
#include "HealthAttributeSet.generated.h"

//
//
// Current Health
// Maximum Health
// Health Regeneration
//
// Damage
// Healing
UCLASS()
class VRPROJECT_API UHealthAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()
	
public:

	UHealthAttributeSet();

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Used to create a local copy of Damage which is then subtracted from Current Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Damage)

	// Used to create a local copy of Healing which is then added to Current Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Healing)

	// Used to create a local copy of Bleed which is then added to Bleeding.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData ReceivedDamageOverTime;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, ReceivedDamageOverTime)

	// Used to create a local copy of Direct Damage which is then added to Current Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData ReceivedDirectDamage;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, ReceivedDirectDamage)

	// Holds the current value for Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, CurrentHealth)

	// Holds the value for Maximum Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_MaximumHealth)
	FGameplayAttributeData MaximumHealth;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaximumHealth)

	// Holds the value for Health Regeneration.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_HealthRegeneration)
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, HealthRegeneration)

	// Holds the current bleeding amount
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_DamageOverTime)
	FGameplayAttributeData DamageOverTime;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, DamageOverTime)

	// Holds the current bleed healing rate
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_DamageOverTimeHealing)
	FGameplayAttributeData DamageOverTimeHealing;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, DamageOverTimeHealing)

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayAttribute, FVRAttributeMaxValue> AttributeMaxValue;

	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaximumHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_DamageOverTime(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_DamageOverTimeHealing(const FGameplayAttributeData& OldValue);
};
