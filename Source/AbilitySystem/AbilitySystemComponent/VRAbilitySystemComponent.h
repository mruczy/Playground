// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilitySystemData.h"
#include "VRAbilitySystemComponent.generated.h"

class ABaseCharacter;

UENUM(BlueprintType)
enum class EVREffectEventType : uint8
{
	Added,
	Removed
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVRGameplayEffectEventDelegate, const EVREffectEventType, EventType, const FActiveGameplayEffect&, Effect);

/**
 * 
 */
UCLASS()
class VRPROJECT_API UVRAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	UVRAbilitySystemComponent();

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	// Called to initialize an Ability System Component with the supplied data. (Can be found in "AbilitySystemData.h")
	// Call this on the Server and Client to properly init references / values.
	UFUNCTION(BlueprintCallable)
	void InitializeAbilitySystemData(const FAbilitySystemInitializationData& InitializationData, AActor* InOwningActor, AActor* InAvatarActor);

	// Wrapper for the "GetOrCreateAttributeSubobject" function. Returns the specified Attribute Set / creates one if it isn't found.
	const UAttributeSet* GetOrCreateAttributeSet(const TSubclassOf<UAttributeSet>& InAttributeSet);

	/**
	 * Update all abilities and effects to reach the newly defined level.
	 * @param DesiredLevel New Level as Float. Will be floored to only Complete level will be taken in account
	 * @param bInitialization Should we initialized All attributes, on only the one that are supposed to be leveled up ?
	 */
	UFUNCTION(Reliable, Server, BlueprintCallable)
	virtual void ChangeLevel(const float DesiredLevel, const bool bInitialization = false);

	ABaseCharacter* GetBaseCharacterAvatar() const;

	virtual FActiveGameplayEffectHandle SetActiveGameplayEffectInhibit(FActiveGameplayEffectHandle&& ActiveGEHandle, bool bInhibit, bool bInvokeGameplayCueEvents) override;

protected:

	bool AbilitySystemDataInitialized = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSet<FGameplayAttribute> NoLevelAttribute{};

	// Callback functions where a GE is added, inhibited or removed.
	virtual void OnGameplayEffectAddedCallback(UAbilitySystemComponent* const ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	virtual void OnGameplayEffectRemovedCallback(const FActiveGameplayEffect& ActiveGameplayEffect);

public:

	/** Returns an attribute value, after applying tag filters */
	UFUNCTION(BlueprintPure)
	float GetFilteredAttribute(const FGameplayAttribute Attribute, const FGameplayTagRequirements SourceTags, const FGameplayTagContainer TargetTags);

	FOnVRGameplayEffectEventDelegate OnVRGameplayEffectEventDelegate;

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual FActiveGameplayEffectHandle ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpec& GameplayEffect, UAbilitySystemComponent* Target, FPredictionKey PredictionKey = FPredictionKey()) override;
};
