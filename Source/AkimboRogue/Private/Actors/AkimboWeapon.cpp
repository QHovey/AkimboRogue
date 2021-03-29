// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AkimboWeapon.h"
#include "AbilitySystem/AkimboGameplayAbility.h"
#include "AbilitySystem/AkimboAbilitySystemComponent.h"
#include "AbilitySystem/AkimboGameplayEffect.h"
#include "AbilitySystem/AkimboGameplayAbility.h"
#include "Attributes/AkimboAttributeSet.h"

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

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
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

void AAkimboWeapon::OnEquippedBy(APawn* Equipper, EHand EquippedToHand)
{
	if (AbilitySystemComponent)
	{
		APlayerController* PC = Equipper->GetController<APlayerController>();
		if (PC)
		{
			EnableInput(PC);
			if (ensure(InputComponent))
			{
				const FGameplayAbilityInputBinds InputBinds("Confirm", "Cancel", "EAkimboAbilityInputID");
				AbilitySystemComponent->BindAbilityActivationToInputComponent(Equipper->InputComponent, InputBinds);
			}
		}

		// Set our ASC as the child of our equippers ASC
		if (IAbilitySystemInterface* IEquipperASC = Cast<IAbilitySystemInterface>(Equipper))
		{
			UAkimboAbilitySystemComponent* EquipperASC = Cast<UAkimboAbilitySystemComponent>(IEquipperASC->GetAbilitySystemComponent());
			if (AbilitySystemComponent) {
				AbilitySystemComponent->SetOwnerASC(EquipperASC);
			}
		}

		GiveAbilities(EquippedToHand);
	}
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

void AAkimboWeapon::GiveAbilities(EHand EquippedToHand)
{
	if (AbilitySystemComponent)
	{
		for (TSubclassOf<UAkimboGameplayAbility>& Ability : DefaultAbilities)
		{
			EAkimboAbilityInputID inputID = EAkimboAbilityInputID::None;
			if (Ability.GetDefaultObject()->IsTriggerAbility && EquippedToHand != EHand::NONE)
			{
				inputID = EquippedToHand == EHand::LEFT ? EAkimboAbilityInputID::ShootLeft : EAkimboAbilityInputID::ShootRight;
			}
			AbilitySystemComponent->GiveAndSlotAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(inputID), this),
				Ability.GetDefaultObject()->DefaultSlot);
		}
	}
}