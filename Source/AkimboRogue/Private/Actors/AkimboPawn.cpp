// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/AkimboPawn.h"
#include "Attributes/AkimboAttributeSet.h"
#include "AbilitySystem/AkimboGameplayAbility.h"
#include "Actors/AkimboWeapon.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

// Sets default values
AAkimboPawn::AAkimboPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAkimboAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	Attributes = CreateDefaultSubobject<UAkimboAttributeSet>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void AAkimboPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AAkimboPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &AAkimboPawn::OnApplyGameplayEffectToSelfCallback);
	}

	InitializeAttributes();
	GiveAbilities();
}

void AAkimboPawn::OnApplyGameplayEffectToSelfCallback(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	ApplyGameplayEffectToWeapon(GetRightWeapon(), SpecApplied);
	ApplyGameplayEffectToWeapon(GetLeftWeapon(), SpecApplied);
}

void AAkimboPawn::ApplyGameplayEffectToWeapon(AAkimboWeapon* Weapon, const FGameplayEffectSpec& SpecApplied)
{
	if (Weapon)
	{
		UAkimboAbilitySystemComponent* ASC = Cast<UAkimboAbilitySystemComponent>(Weapon->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->ApplyGameplayEffectSpecToSelf(SpecApplied);
		}
	}
}

void AAkimboPawn::InitializeAttributes()
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

void AAkimboPawn::GiveAbilities()
{
	if (AbilitySystemComponent)
	{
		for (TSubclassOf<UAkimboGameplayAbility>& Ability : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAndSlotAbility(FGameplayAbilitySpec(Ability, 1, 0, this),
				Ability.GetDefaultObject()->DefaultSlot);
		}
	}
}

// Called every frame
void AAkimboPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAkimboPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(InputComponent);

	//InputComponent->Bind

	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AAkimboPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAkimboPawn::SetRightWeapon(class AAkimboWeapon* InWeapon)
{
	RightWeapon = InWeapon;
	OnRightWeaponEquipped(RightWeapon);
}

void AAkimboPawn::SetLeftWeapon(class AAkimboWeapon* InWeapon)
{
	LeftWeapon = InWeapon;
	OnLeftWeaponEquipped(LeftWeapon);
}

class AAkimboWeapon* AAkimboPawn::GetRightWeapon()
{
	return RightWeapon;
}

class AAkimboWeapon* AAkimboPawn::GetLeftWeapon()
{
	return LeftWeapon;
}
