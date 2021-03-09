// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Abilities/GameplayAbilityTypes.h>
#include "AkimboGameplayAbilityActorInfo.generated.h"

/**
 * 
 */
 USTRUCT(BlueprintType)
struct AKIMBOROGUE_API FAkimboGameplayAbilityActorInfo : public FGameplayAbilityActorInfo
{
	GENERATED_USTRUCT_BODY()

public:

	// FGameplayAbilityActorInfo
	void InitFromActor(AActor* OwnerActor, AActor* AvatarActor, UAbilitySystemComponent* InAbilitySystemComponent) override;

public:

	/** We can create chains of Actors that all have ASCs individually. This pointer points to the root Actor of the chain.
	* Follows the AvatarActor path.
	* Example: 
	*	Player (Root)
	*		Weapon
	*			AoE
	* **/
	UPROPERTY(BlueprintReadOnly, Category = "ActorInfo")
	UAbilitySystemComponent* RootOwnerASC;

};
