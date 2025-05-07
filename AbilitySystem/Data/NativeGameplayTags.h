// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/GameplayTags/Public/NativeGameplayTags.h"

namespace NativeGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_NoLevel);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Type_Cooldown);

	namespace CharacterTags
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_PC);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_NPC);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_State_BlockHealthRegen);
	}

	namespace BasicAttack
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageType_DirectDamage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Modifier_DirectDamage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Ignore_DirectDamage);
	}

	namespace State
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Health);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Stamina);

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Health);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Stamina);
	}

	namespace Statuses
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Debuff_DamageOverTime);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageType_DamageOverTime);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Immunity_DamageOverTime);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Modifier_DamageOverTime);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Ignore_DamageOverTime);
	}
}