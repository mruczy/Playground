// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AbilitySystemWidget.h"
#include "AbilitySystem/AbilitySystemComponent/VRAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/HealthAttributeSet.h"
#include "AbilitySystem/AttributeSets/StaminaAttributeSet.h"
#include "AbilitySystem/AttributeSets/LevelAttributeSet.h"
#include "AbilitySystem/AttributeSets/ResistanceAttributeSet.h"

bool UAbilitySystemWidget::InitializeAbilitySystemWidget(UAbilitySystemComponent* InOwnerAbilitySystemComponent)
{
	UAbilitySystemComponent* OldAbilitySystemComponent = AbilitySystemComponent.Get();
	AbilitySystemComponent = Cast<UVRAbilitySystemComponent>(InOwnerAbilitySystemComponent);

	// The Ability System Component is invalid. Stop here and return false. 
	if (!GetOwnerAbilitySystemComponent())
	{
		return false;
	}

	// Reset any old Attribute Change Delegates if they are still bound.
	if (IsValid(OldAbilitySystemComponent))
	{
		ResetDelegateHandle(MaximumHealthChangeDelegate, OldAbilitySystemComponent, UHealthAttributeSet::GetMaximumHealthAttribute());
		ResetDelegateHandle(CurrentHealthChangeDelegate, OldAbilitySystemComponent, UHealthAttributeSet::GetCurrentHealthAttribute());
		ResetDelegateHandle(HealthRegenerationChangeDelegate, OldAbilitySystemComponent, UHealthAttributeSet::GetHealthRegenerationAttribute());
		ResetDelegateHandle(MaximumStaminaChangeDelegate, OldAbilitySystemComponent, UStaminaAttributeSet::GetMaximumStaminaAttribute());
		ResetDelegateHandle(CurrentStaminaChangeDelegate, OldAbilitySystemComponent, UStaminaAttributeSet::GetCurrentStaminaAttribute());
		ResetDelegateHandle(StaminaRegenerationChangeDelegate, OldAbilitySystemComponent, UStaminaAttributeSet::GetStaminaRegenerationAttribute());
		ResetDelegateHandle(LevelChangeDelegate, OldAbilitySystemComponent, ULevelAttributeSet::GetCurrentLevelAttribute());
		ResetDelegateHandle(ResistanceChangeDelegate, OldAbilitySystemComponent, UResistanceAttributeSet::GetResistanceAttribute());
		ResetDelegateHandle(DamageOverTimeChangeDelegate, OldAbilitySystemComponent, UHealthAttributeSet::GetDamageOverTimeAttribute());
		ResetDelegateHandle(DamageOverTimeHealChangeDelegate, OldAbilitySystemComponent, UHealthAttributeSet::GetDamageOverTimeHealingAttribute());
	}

	bool bBindingDone = false;

	// Bind Health attribute delegates if the Ability System Component has the required Attribute Set -and- we are listening for Health attributes.
	if (bListenForAttributeChanges)
	{
		if (AbilitySystemComponent->HasAttributeSetForAttribute(UHealthAttributeSet::GetMaximumHealthAttribute()))
		{
			bBindingDone = true;

			MaximumHealthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetMaximumHealthAttribute()).AddUObject(this, &UAbilitySystemWidget::MaximumHealthChanged);
			CurrentHealthChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetCurrentHealthAttribute()).AddUObject(this, &UAbilitySystemWidget::CurrentHealthChanged);
			HealthRegenerationChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetHealthRegenerationAttribute()).AddUObject(this, &UAbilitySystemWidget::HealthRegenerationChanged);
			ResistanceChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UResistanceAttributeSet::GetResistanceAttribute()).AddUObject(this, &UAbilitySystemWidget::ResistanceChanged);
			DamageOverTimeChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetDamageOverTimeAttribute()).AddUObject(this, &UAbilitySystemWidget::DamageOverTimeChanged);
			DamageOverTimeHealChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetDamageOverTimeHealingAttribute()).AddUObject(this, &UAbilitySystemWidget::DamageOverTimeChanged);

			const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(UHealthAttributeSet::GetMaximumHealthAttribute());
			const float CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UHealthAttributeSet::GetCurrentHealthAttribute());

			// Call the Blueprint Events to initialize the values
			On_MaximumHealthChanged(MaxHealth, 0.0f, CurrentHealth / MaxHealth);
			On_CurrentHealthChanged(CurrentHealth, 0.0f, CurrentHealth / MaxHealth);
			On_HealthRegenerationChanged(AbilitySystemComponent->GetNumericAttribute(UHealthAttributeSet::GetHealthRegenerationAttribute()), 0.0f);

			DamageOverTimeChanged(FOnAttributeChangeData());
		}

		if (AbilitySystemComponent->HasAttributeSetForAttribute(UStaminaAttributeSet::GetMaximumStaminaAttribute()))
		{
			bBindingDone = true;

			MaximumStaminaChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStaminaAttributeSet::GetMaximumStaminaAttribute()).AddUObject(this, &UAbilitySystemWidget::MaximumStaminaChanged);
			CurrentStaminaChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStaminaAttributeSet::GetCurrentStaminaAttribute()).AddUObject(this, &UAbilitySystemWidget::CurrentStaminaChanged);
			StaminaRegenerationChangeDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UStaminaAttributeSet::GetStaminaRegenerationAttribute()).AddUObject(this, &UAbilitySystemWidget::StaminaRegenerationChanged);

			const float MaxStamina = AbilitySystemComponent->GetNumericAttribute(UStaminaAttributeSet::GetMaximumStaminaAttribute());
			const float CurrentStamina = AbilitySystemComponent->GetNumericAttribute(UStaminaAttributeSet::GetCurrentStaminaAttribute());

			// Call the Blueprint Events to initialize the values
			On_MaximumStaminaChanged(MaxStamina, 0.0f, CurrentStamina / MaxStamina);
			On_CurrentStaminaChanged(CurrentStamina, 0.0f, CurrentStamina / MaxStamina);
			On_StaminaRegenerationChanged(AbilitySystemComponent->GetNumericAttribute(UStaminaAttributeSet::GetStaminaRegenerationAttribute()), 0.0f);
		}
	}

	K2_InitializeAbilitySystemWidget(bBindingDone);

	return bBindingDone;
}

UAbilitySystemComponent* UAbilitySystemWidget::GetOwnerAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void UAbilitySystemWidget::CurrentLevelChanged(const FOnAttributeChangeData& Data)
{
	On_LevelChanged(Data.NewValue, Data.OldValue);
}

void UAbilitySystemWidget::MaximumHealthChanged(const FOnAttributeChangeData& Data)
{
	const float CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UHealthAttributeSet::GetCurrentHealthAttribute());

	On_MaximumHealthChanged(Data.NewValue, Data.OldValue, Data.NewValue > 0.f ? CurrentHealth / Data.NewValue : 0.f);
}

void UAbilitySystemWidget::CurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(UHealthAttributeSet::GetMaximumHealthAttribute());

	On_CurrentHealthChanged(Data.NewValue, Data.OldValue, MaxHealth > 0.f ? Data.NewValue / MaxHealth : 0.f);
}

void UAbilitySystemWidget::HealthRegenerationChanged(const FOnAttributeChangeData& Data)
{
	On_HealthRegenerationChanged(Data.NewValue, Data.OldValue);
}

void UAbilitySystemWidget::ResistanceChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	On_ResistanceChanged(AbilitySystemComponent->GetNumericAttributeBase(Data.Attribute), Data.NewValue);
}

void UAbilitySystemWidget::MaximumStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	const float CurrentStamina = AbilitySystemComponent->GetNumericAttribute(UStaminaAttributeSet::GetCurrentStaminaAttribute());

	On_MaximumStaminaChanged(Data.NewValue, Data.OldValue, Data.NewValue > 0.f ? CurrentStamina / Data.NewValue : 0.f);
}

void UAbilitySystemWidget::CurrentStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	const float MaxStamina = AbilitySystemComponent->GetNumericAttribute(UStaminaAttributeSet::GetMaximumStaminaAttribute());

	On_CurrentStaminaChanged(Data.NewValue, Data.OldValue, MaxStamina > 0.f ? Data.NewValue / MaxStamina : 0.f);
}

void UAbilitySystemWidget::StaminaRegenerationChanged(const FOnAttributeChangeData& Data)
{
	On_StaminaRegenerationChanged(Data.NewValue, Data.OldValue);
}

void UAbilitySystemWidget::DamageOverTimeChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	const float DamageOverTime = AbilitySystemComponent->GetNumericAttribute(UHealthAttributeSet::GetDamageOverTimeAttribute());
	const float DamageOverTimeHeal = AbilitySystemComponent->GetNumericAttribute(UHealthAttributeSet::GetDamageOverTimeHealingAttribute());

	const float Duration = DamageOverTimeHeal > 0 ? DamageOverTime / DamageOverTimeHeal : 0.f;

	On_DamageOverTimeChanged(DamageOverTime, DamageOverTimeHeal, Duration);
}

void UAbilitySystemWidget::ResetDelegateHandle(FDelegateHandle DelegateHandle, UAbilitySystemComponent* OldAbilitySystemComponent, const FGameplayAttribute& Attribute)
{
	if (IsValid(OldAbilitySystemComponent))
	{
		OldAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(DelegateHandle);
		DelegateHandle.Reset();
	}
}
