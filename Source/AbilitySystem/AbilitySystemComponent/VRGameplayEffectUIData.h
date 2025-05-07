// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "VRGameplayEffectUIData.generated.h"

class UEffectWidgetControllerBase;
struct FActiveGameplayEffectHandle;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EVRGEDisplayParameter : uint8
{
	NONE			= 0 UMETA(Hidden),
	HIDE_COOLDOWN   = 1 << 0
};
ENUM_CLASS_FLAGS(EVRGEDisplayParameter);

/**
 * 
 */
UCLASS(DisplayName = "UI Data", Config = Game)
class VRPROJECT_API UVRGameplayEffectUIData : public UGameplayEffectUIData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Data, meta = (MultiLine = "true"))
	FText Title;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Data, meta = (MultiLine = "true"))
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Data)
	TSoftObjectPtr<UTexture2D> IconTexture;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Data)
	TSoftObjectPtr<UMaterialInterface> IconMaterial;

	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Data)
	//TSubclassOf<UEffectWidgetControllerBase> EffectWidgetController;


};
