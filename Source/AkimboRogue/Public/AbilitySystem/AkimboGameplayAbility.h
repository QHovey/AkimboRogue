// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AkimboAbilitySystemComponent.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAkimboAbilityEnded, class UAkimboGameplayAbility*, Ability, bool, WasCanceled);

/**
 * 
 */
UCLASS()
class AKIMBOROGUE_API UAkimboGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UAkimboGameplayAbility(const FObjectInitializer& ObjectInitializer);

	//////////////////////////////////////////////////////////////////////////
	// UGameplayAbility

	void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

public:

	// Retrieve an array of sub-abilities this ability relies on. Used to chain abilities together.
	// Must implement and return a TSubClassOf<UGameplayAbility> if you attempt to call ASC->TryActivateAbility from within this ability.
	UFUNCTION(BlueprintImplementableEvent)
	void GetSubAbilities(TArray<TSubclassOf<UGameplayAbility>>& OutSubAbilities);

	/** Returns the AkimboAbilitySystemComponent that is activating this ability */
	UFUNCTION(BlueprintCallable, Category = Ability)
	UAkimboAbilitySystemComponent* GetAkimboAbilitySystemComponent() const;

public:

	/** This ability can only be activated if the root owner of the ASC chain has all of these tags */
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta = (Categories = "SourceTagsCategory"))
	FGameplayTagContainer OwnerRequiredTags;

	// This ability is blocked if the root owner of the ASC chain has any of these tags
	UPROPERTY(EditDefaultsOnly, Category = Tags, meta = (Categories = "RootOwnerTagsCategory"))
	FGameplayTagContainer OwnerBlockedTags;

public:

	// InputID to assign this ability to by default. Enum name must match an Input name in the Project Settings->Input mappings
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Input")
	EAkimboAbilityInputID AbilityInputID = EAkimboAbilityInputID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Input")
	EAbilitySlot DefaultSlot = EAbilitySlot::SLOT_INVALID;
	
	// Is this ability triggered directly by input. This is used primarily by Weapons who'll assign it's own input ID to the ability spec.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability|Input")
	bool IsTriggerAbility = false;

	UPROPERTY(BlueprintAssignable, Category = "Ability|Event")
	FOnAkimboAbilityEnded OnAbilityEnded;

private:

	// Keeps track of the sub abilities we added to our ASC
	TArray<FGameplayAbilitySpecHandle> m_subAbilityHandles;

protected:
	

};
