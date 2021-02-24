// Fill out your copyright notice in the Description page of Project Settings.


#include "AkimboCharacter.h"
#include "AkimboAbilitySystemComponent.h"
#include "AkimboAttributeSet.h"
#include "AkimboGameplayAbility.h"
#include "AkimboGameplayEffect.h"

// Sets default values
AAkimboCharacter::AAkimboCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Ability System
	AbilitySystemComponent = CreateDefaultSubobject<UAkimboAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	Attributes = CreateDefaultSubobject<UAkimboAttributeSet>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void AAkimboCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAkimboCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAkimboCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	InitializeAttributes();
	GiveAbilities();
}

UAbilitySystemComponent* AAkimboCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAkimboCharacter::InitializeAttributes()
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

void AAkimboCharacter::GiveAbilities()
{
	if (AbilitySystemComponent)
	{
		for (TSubclassOf<UAkimboGameplayAbility>& Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
		}
	}
}
