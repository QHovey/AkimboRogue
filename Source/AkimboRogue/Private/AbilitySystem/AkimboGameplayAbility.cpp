// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AkimboGameplayAbility.h"
#include <AbilitySystemComponent.h>
#include <AbilitySystemGlobals.h>
#include "AbilitySystem/AkimboAbilitySystemComponent.h"

UAkimboGameplayAbility::UAkimboGameplayAbility(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAkimboGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (ActorInfo->AbilitySystemComponent != nullptr) {
		TArray<TSubclassOf<UGameplayAbility>> subAbilities;
		GetSubAbilities(subAbilities);
		for (TSubclassOf<UGameplayAbility>& sub : subAbilities) {
			m_subAbilityHandles.Add(ActorInfo->AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(sub, Spec.Level, -1, Spec.SourceObject)));
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

	bool bBlocked = false;
	bool bMissing = false;

	if (UAkimboAbilitySystemComponent* AkimboASC = Cast<UAkimboAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get()))
	{
		UAkimboAbilitySystemComponent* RootOwnerASC = AkimboASC->GetRootOwnerASC();
		if (OwnerBlockedTags.Num())
		{
			if (RootOwnerASC->HasAnyMatchingGameplayTags(OwnerBlockedTags))
			{
				bBlocked = true;
			}
		}
		if (OwnerRequiredTags.Num())
		{
			if (!RootOwnerASC->HasAllMatchingGameplayTags(OwnerRequiredTags))
			{
				bMissing = true;
			}
		}

		static FGameplayTagContainer AbilitySystemComponentTags;
		AbilitySystemComponentTags.Reset();

		RootOwnerASC->GetOwnedGameplayTags(AbilitySystemComponentTags);

		// Add any more blocked / missing checks here

		// Check fail cases, output optional reason tag, and return
		UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
		if (bBlocked)
		{
			if (OptionalRelevantTags && AbilitySystemGlobals.ActivateFailTagsBlockedTag.IsValid())
			{
				OptionalRelevantTags->AddTag(AbilitySystemGlobals.ActivateFailTagsBlockedTag);
			}
			return false;
		}
		if (bMissing)
		{
			if (OptionalRelevantTags && AbilitySystemGlobals.ActivateFailTagsMissingTag.IsValid())
			{
				OptionalRelevantTags->AddTag(AbilitySystemGlobals.ActivateFailTagsMissingTag);
			}
			return false;
		}
	}

	return true;
}

void UAkimboGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

void UAkimboGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	OnAbilityEnded.Broadcast(this, bWasCancelled);
}

UAkimboAbilitySystemComponent* UAkimboGameplayAbility::GetAkimboAbilitySystemComponent() const
{
	if (!ensure(CurrentActorInfo))
	{
		return nullptr;
	}
	return Cast<UAkimboAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get());
}
