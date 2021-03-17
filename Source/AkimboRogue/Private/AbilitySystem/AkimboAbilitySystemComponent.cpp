// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AkimboAbilitySystemComponent.h"
#include "AbilitySystem/AkimboGameplayAbility.h"
#include <GameplayAbilitySpec.h>

UAkimboAbilitySystemComponent::UAkimboAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
{
	SlottedAbilities.AddDefaulted((uint8)EAbilitySlot::SLOT_COUNT);
}

/////////////////////////////////////////////////////////////////////////
// Copy-Paste override implementation of the tow BindToInputComponent methods so that I can turn off the INFURIATING 
// bConsumeInput on the action binding. FUCKING STOP MAKING THAT DEFAULT TO TRUE EPIC!!!!
void UAkimboAbilitySystemComponent::BindToInputComponent(UInputComponent* InputComponent)
{
	static const FName ConfirmBindName(TEXT("AbilityConfirm"));
	static const FName CancelBindName(TEXT("AbilityCancel"));

	// Pressed event
	{
		FInputActionBinding AB(ConfirmBindName, IE_Pressed);
		AB.bConsumeInput = false; // This is all I really wanted
		AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::LocalInputConfirm);
		InputComponent->AddActionBinding(AB);
	}

	// 
	{
		FInputActionBinding AB(CancelBindName, IE_Pressed);
		AB.bConsumeInput = false; // This is all I really wanted
		AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::LocalInputCancel);
		InputComponent->AddActionBinding(AB);
	}
}

void UAkimboAbilitySystemComponent::BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo)
{
	UEnum* EnumBinds = BindInfo.GetBindEnum();

	SetBlockAbilityBindingsArray(BindInfo);

	for (int32 idx = 0; idx < EnumBinds->NumEnums(); ++idx)
	{
		const FString FullStr = EnumBinds->GetNameStringByIndex(idx);

		// Pressed event
		{
			FInputActionBinding AB(FName(*FullStr), IE_Pressed);
			AB.bConsumeInput = false; // This is all I really wanted
			AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::AbilityLocalInputPressed, idx);
			InputComponent->AddActionBinding(AB);
		}

		// Released event
		{
			FInputActionBinding AB(FName(*FullStr), IE_Released);
			AB.bConsumeInput = false; // This is all I really wanted
			AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::AbilityLocalInputReleased, idx);
			InputComponent->AddActionBinding(AB);
		}
	}

	// Bind Confirm/Cancel. Note: these have to come last!
	if (BindInfo.ConfirmTargetCommand.IsEmpty() == false)
	{
		FInputActionBinding AB(FName(*BindInfo.ConfirmTargetCommand), IE_Pressed);
		AB.bConsumeInput = false; // This is all I really wanted
		AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::LocalInputConfirm);
		InputComponent->AddActionBinding(AB);
	}

	if (BindInfo.CancelTargetCommand.IsEmpty() == false)
	{
		FInputActionBinding AB(FName(*BindInfo.CancelTargetCommand), IE_Pressed);
		AB.bConsumeInput = false; // This is all I really wanted
		AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::LocalInputCancel);
		InputComponent->AddActionBinding(AB);
	}

	if (BindInfo.CancelTargetInputID >= 0)
	{
		GenericCancelInputID = BindInfo.CancelTargetInputID;
	}
	if (BindInfo.ConfirmTargetInputID >= 0)
	{
		GenericConfirmInputID = BindInfo.ConfirmTargetInputID;
	}
}

void UAkimboAbilitySystemComponent::GiveAndSlotAbility(const FGameplayAbilitySpec& Spec, EAbilitySlot Slot)
{
	FGameplayAbilitySpecHandle handle = GiveAbility(Spec);
	if (handle.IsValid() && Slot != EAbilitySlot::SLOT_INVALID)
	{
		SlottedAbilities[(uint8)Slot] = handle;
	}
}

void UAkimboAbilitySystemComponent::RemoveSlottedAbility(EAbilitySlot Slot)
{
	static FGameplayAbilitySpecHandle InvalidHandle = FGameplayAbilitySpecHandle();
	FGameplayAbilitySpecHandle& handle = SlottedAbilities[(uint8)Slot];
	if (handle.IsValid())
	{
		ClearAbility(handle);
		SlottedAbilities[(uint8)Slot] = InvalidHandle;
	}
}

UAkimboAbilitySystemComponent* UAkimboAbilitySystemComponent::GetRootOwnerASC()
{
	if (OwnerAbilitySystem.IsValid())
	{
		return OwnerAbilitySystem->GetRootOwnerASC();
	}
	return this;
}

bool UAkimboAbilitySystemComponent::TryActivateAkimboAbilityByClass(TSubclassOf<UAkimboGameplayAbility> Ability, UAkimboGameplayAbility*& ActiveAbility)
{
	FGameplayAbilitySpec* SpecPtr = FindAbilitySpecFromClass(Ability);
	if (SpecPtr)
	{
		FGameplayAbilitySpec& Spec = *SpecPtr;
		return activateAkimboAbility(Spec, ActiveAbility);
	}
	return false;
}

bool UAkimboAbilitySystemComponent::TryActivateAkimboAbilityBySlot(EAbilitySlot Slot, UAkimboGameplayAbility*& ActiveAbility)
{
	FGameplayAbilitySpec* SpecPtr = FindAbilitySpecFromHandle(SlottedAbilities[(uint8)Slot]);
	if (SpecPtr)
	{
		FGameplayAbilitySpec& Spec = *SpecPtr;
		return activateAkimboAbility(Spec, ActiveAbility);
	}
	return false;
}

bool UAkimboAbilitySystemComponent::activateAkimboAbility(const FGameplayAbilitySpec& Spec, UAkimboGameplayAbility*& ActiveAbility)
{
	bool bSuccess = TryActivateAbility(Spec.Handle, false);
	if (bSuccess)
	{
		// Ability successfully activated.
		// 
		// Ability could end immediately in a single frame, in which case ActiveCount will be 0
		if (Spec.ActiveCount > 0)
		{
			if (Spec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
			{
				ActiveAbility = CastChecked<UAkimboGameplayAbility>(Spec.Ability);
			}
			else
			{
				ActiveAbility = CastChecked<UAkimboGameplayAbility>(Spec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::InstancedPerActor ?
					Spec.NonReplicatedInstances[0] : Spec.NonReplicatedInstances[Spec.NonReplicatedInstances.Num() - 1]);
			}
		}
		return bSuccess;
	}
	return false;
}
