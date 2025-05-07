// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "ScalableFloat.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "AbilitySystemData.generated.h"

class UGameplayEffect;

USTRUCT(Blueprintable)
struct FVRAttributeMaxValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat MaxFloat{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute MaxAttribute{};
};

USTRUCT(Blueprintable)
struct FVRAttributeInitializer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Value;
};

// Data used to initialize AbilitySystemComponent
USTRUCT(BlueprintType)
struct FAbilitySystemInitializationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSet<TSubclassOf<UAttributeSet>> AttributeSets;

	// Attributes / float used to set base values
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FGameplayAttribute, FVRAttributeInitializer> AttributeBaseValues;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSet<TSubclassOf<UGameplayAbility>> GameplayAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSet<TSubclassOf<UGameplayEffect>> GameplayEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTagContainer GameplayTags;
};

// Returns the value of an Attribute based on the search type.
UENUM(BlueprintType)
enum class EAttributeSearchType : uint8
{
	// Returns the final value of the Attribute including all stateful Gameplay Effect modifiers.
	FinalValue,

	// Returns the base value of the Attribute. (Excludes duration based Gameplay Effect modifiers)
	BaseValue,

	// Returns the Final Value minus the Base Value.
	BonusValue
};


USTRUCT(BlueprintType)
struct FVRContextDataBase
{
	GENERATED_BODY()

	virtual ~FVRContextDataBase() = default;

	/** Overridden to serialize new fields */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		return true;
	};

	FVRContextDataBase() {}
};

USTRUCT(BlueprintType)
struct FVRGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	const TArray<FInstancedStruct>& GetAllVRContextData() const { return VRContextData; }
	TArray<FInstancedStruct>& GetMutableAllVRContextData() { return VRContextData; }

protected:
	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TArray<FInstancedStruct> VRContextData{};

	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TObjectPtr<const AActor> TargetActor;

public:

	void SetTargetActor(const AActor* InTargetActor)
	{
		TargetActor = InTargetActor;
	}

	const AActor* GetTargetActor() const
	{
		return TargetActor.Get();
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	template <typename T>
	const T* FindVRContextData() const
	{
		for (const auto& Fragment : VRContextData)
		{
			if (const T* TypedFragment = Fragment.GetPtr<T>())
				return TypedFragment;
		}

		return nullptr;
	}

	/** Adds a Context Fragment to the ContextFragments array */
	template <typename T>
	void AddVRContextData(const T& Fragment)
	{
		TInstancedStruct<FVRContextDataBase> InstancedStruct;
		InstancedStruct.InitializeAs<T>();
		T& Mutable = InstancedStruct.GetMutable<T>();
		Mutable = Fragment;

		VRContextData.Add(MoveTemp(InstancedStruct));
	}

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FVRGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FVRGameplayEffectContext* Duplicate() const override
	{
		FVRGameplayEffectContext* NewContext = new FVRGameplayEffectContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);

		NewContext->TargetActor = TargetActor;

		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
};