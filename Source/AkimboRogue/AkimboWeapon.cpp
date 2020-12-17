// Fill out your copyright notice in the Description page of Project Settings.


#include "AkimboWeapon.h"
#include "AkimboGameplayAbility.h"
#include "AkimboAbilitySystemComponent.h"
#include "AkimboGameplayEffect.h"
#include "AkimboGameplayAbility.h"
#include "AkimboAttributeSet.h"

// Sets default values
AAkimboWeapon::AAkimboWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAkimboAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	Attributes = CreateDefaultSubobject<UAkimboAttributeSet>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void AAkimboWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	InitializeAttributes();
	GiveAbilities();
}

// Called every frame
void AAkimboWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* AAkimboWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAkimboWeapon::InitializeAttributes()
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

void AAkimboWeapon::GiveAbilities()
{
	if (AbilitySystemComponent)
	{
		for (TSubclassOf<UAkimboGameplayAbility>& Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, 0, this));
		}
	}
}