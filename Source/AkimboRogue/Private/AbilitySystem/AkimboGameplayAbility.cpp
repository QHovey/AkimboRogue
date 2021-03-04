// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AkimboGameplayAbility.h"

void UAkimboGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	//throw std::logic_error("The method or operation is not implemented.");
}

void UAkimboGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
	//throw std::logic_error("The method or operation is not implemented.");
}
