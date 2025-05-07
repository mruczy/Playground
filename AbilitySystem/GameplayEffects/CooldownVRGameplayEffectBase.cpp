// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayEffects/CooldownVRGameplayEffectBase.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "AbilitySystem/Data/NativeGameplayTags.h"
#include "AbilitySystem/AttributeSets/AbilitiesAttributeSet.h"

UCooldownVRGameplayEffectBase::UCooldownVRGameplayEffectBase()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 1;
}

void UCooldownVRGameplayEffectBase::PostLoad()
{
	Super::PostLoad();
	ConfigureEffect();
}

void UCooldownVRGameplayEffectBase::PostInitProperties()
{
	Super::PostInitProperties();
	ConfigureEffect();
}

void UCooldownVRGameplayEffectBase::ConfigureEffect()
{
	UAssetTagsGameplayEffectComponent& AssetTagComponent = FindOrAddComponent<UAssetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTags{};
	InheritedTags.AddTag(NativeGameplayTags::TAG_Effect_Type_Cooldown.GetTag());
	AssetTagComponent.SetAndApplyAssetTagChanges(InheritedTags);

	FAttributeBasedFloat AttributeBaseCooldown{};
	AttributeBaseCooldown.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeMagnitude;
	AttributeBaseCooldown.BackingAttribute.AttributeToCapture = UAbilitiesAttributeSet::GetCooldownMultiplierAttribute();
	AttributeBaseCooldown.BackingAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;

	DurationMagnitude = AttributeBaseCooldown;
}
