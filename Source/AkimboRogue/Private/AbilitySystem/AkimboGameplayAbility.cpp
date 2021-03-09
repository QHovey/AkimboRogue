// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AkimboGameplayAbility.h"
#include <AbilitySystemComponent.h>

void UAkimboGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (ActorInfo->AbilitySystemComponent != nullptr) {
		TArray<TSubclassOf<UGameplayAbility>> subAbilities;
		GetSubAbilities(subAbilities);
		for (TSubclassOf<UGameplayAbility>& sub : subAbilities) {
			m_subAbilityHandles.Add(ActorInfo->AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(sub, Spec.Level, Spec.InputID, Spec.SourceObject)));
		}
	}
}

void UAkimboGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	if (ActorInfo->AbilitySystemComponent != nullptr) {
		for (FGameplayAbilitySpecHandle& handle : m_subAbilityHandles) {
			ActorInfo->AbilitySystemComponent->ClearAbility(handle);
		}
	}
}

bool UAkimboGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /*= nullptr*/, const FGameplayTagContainer* TargetTags /*= nullptr*/, OUT FGameplayTagContainer* OptionalRelevantTags /*= nullptr*/) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, nullptr, nullptr, nullptr))
	{
		return false;
	}

	return true;
}

void UAkimboGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

bool UAkimboGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags /*= nullptr*/, const FGameplayTagContainer* TargetTags /*= nullptr*/, OUT FGameplayTagContainer* OptionalRelevantTags /*= nullptr*/) const
{
	if (!Super::DoesAbilitySatisfyTagRequirements(AbilitySystemComponent, nullptr, nullptr, nullptr))
	{
		return false;
	}

	return true;
}
