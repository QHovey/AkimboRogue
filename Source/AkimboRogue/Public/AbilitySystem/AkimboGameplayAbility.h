// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AkimboGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EAkimboAbilityInputID : uint8
{
	// 0 None
	None				UMETA(DisplayName = "None"),
	// 1 
	ShootLeft			UMETA(DisplayName = "Shoot Left"),
	// 2
	ShootRight			UMETA(DisplayName = "Shoot Right")
};

/**
 * 
 */
UCLASS()
class AKIMBOROGUE_API UAkimboGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	//////////////////////////////////////////////////////////////////////////
	// UGameplayAbility

	void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

public:

	// Retrieve an array of sub-abilities this ability relies on. Used to chain abilities together.
	// Must implement and return a TSubClassOf<UGameplayAbility> if you attempt to call ASC->TryActivateAbility from within this ability.
	UFUNCTION(BlueprintImplementableEvent)
	void GetSubAbilities(TArray<TSubclassOf<UGameplayAbility>>& OutSubAbilities);

public:

	/** This ability is blocked if the source pawn/character has any of these tags. Typically the owner of a weapon */
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta = (Categories = "SourceTagsCategory"))
	FGameplayTagContainer SourcePawnBlockedTags;

public:

	// InputID to assign this ability to by default. Enum name must match an Input name in the Project Settings->Input mappings
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAkimboAbilityInputID AbilityInputID = EAkimboAbilityInputID::None;
	
	// Is this ability triggered directly by input. This is used primarily by Weapons who'll assign it's own input ID to the ability spec.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	bool IsTriggerAbility = false;

private:

	TArray<FGameplayAbilitySpecHandle> m_subAbilityHandles;

};
