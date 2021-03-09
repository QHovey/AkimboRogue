// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AkimboGameplayAbilityActorInfo.h"
#include <GameFramework/Actor.h>

void FAkimboGameplayAbilityActorInfo::InitFromActor(AActor* InOwnerActor, AActor* InAvatarActor, UAbilitySystemComponent* InAbilitySystemComponent)
{
	Super::InitFromActor(InOwnerActor, InAvatarActor, InAbilitySystemComponent);
}
