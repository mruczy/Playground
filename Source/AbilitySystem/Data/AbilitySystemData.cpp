// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilitySystemData.h"

bool FVRGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    bool bCombinedSuccess = FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

    enum RepFlag
    {
        REP_VRContextData,
        REP_TargetActor,
        REP_MAX
    };

    uint16 RepBits = 0;
    if (Ar.IsSaving())
    {
        if (!VRContextData.IsEmpty())
        {
            RepBits |= 1 << REP_VRContextData;
        }

        if (TargetActor)
        {
            RepBits |= 1 << REP_TargetActor;
        }
    }

    Ar.SerializeBits(&RepBits, REP_MAX);
    if (RepBits & (1 << REP_VRContextData))
    {
        bCombinedSuccess &= SafeNetSerializeTArray_WithNetSerialize<31>(Ar, VRContextData, Map);
    }
    if (RepBits & (1 << REP_TargetActor))
    {
        Ar << TargetActor;
    }

    return bCombinedSuccess;
}
