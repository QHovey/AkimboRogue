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

public:

	// InputID to assign this ability to by default. Enum name must match an Input name in the Project Settings->Input mappings
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAkimboAbilityInputID AbilityInputID = EAkimboAbilityInputID::None;
	
	// Is this ability triggered directly by input. This is used primarily by Weapons who'll assign it's own input ID to the ability spec.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	bool IsTriggerAbility = false;
};
