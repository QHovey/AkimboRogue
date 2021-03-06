// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AkimboActor.h"
#include "AbilitySystem/AkimboAbilitySystemComponent.h"
#include "AbilitySystem/AkimboGameplayEffect.h"
#include "Attributes/AkimboAttributeSet.h"

// Sets default values
AAkimboActor::AAkimboActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAkimboAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	Attributes = CreateDefaultSubobject<UAkimboAttributeSet>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void AAkimboActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeAttributes();
	GiveAbilities();
}

// Called every frame
void AAkimboActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

class UAbilitySystemComponent* AAkimboActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAkimboActor::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContextHandle);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AAkimboActor::GiveAbilities()
{
	if (AbilitySystemComponent)
	{
		for (TSubclassOf<UAkimboGameplayAbility>& Ability : DefaultAbilities)
		{
			EAkimboAbilityInputID inputID = Ability.GetDefaultObject()->AbilityInputID;
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(inputID), this));
		}
	}
}

