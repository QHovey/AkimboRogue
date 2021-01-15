// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "AkimboAbilitySystemComponent.h"
#include "AkimboGameplayEffect.h"
#include "AkimboPawn.generated.h"

UCLASS()
class AKIMBOROGUE_API AAkimboPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAkimboPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	// Callback for when a GameplayEffect is applied to ourselves
	void OnApplyGameplayEffectToSelfCallback(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	void ApplyGameplayEffectToWeapon(AAkimboWeapon* Weapon, const FGameplayEffectSpec& SpecApplied);

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ~AbilitySystemInterface
public:

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:

	UFUNCTION(BlueprintCallable)
	void SetRightWeapon(class AAkimboWeapon* InWeapon);
	UFUNCTION(BlueprintImplementableEvent)
	void OnRightWeaponEquipped(class AAkimboWeapon* Weapon);

	UFUNCTION(BlueprintCallable)
	void SetLeftWeapon(class AAkimboWeapon* InWeapon);
	UFUNCTION(BlueprintImplementableEvent)
	void OnLeftWeaponEquipped(class AAkimboWeapon* Weapon);

public:

	UPROPERTY(BlueprintReadWrite)
	class UAkimboAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UAkimboAttributeSet* Attributes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<class UAkimboGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<class UAkimboGameplayAbility>> DefaultAbilities;

public:

	UFUNCTION(BlueprintCallable)
	class AAkimboWeapon* GetRightWeapon();
	UFUNCTION(BlueprintCallable)
	class AAkimboWeapon* GetLeftWeapon();
protected:

	class AAkimboWeapon* RightWeapon;
	class AAkimboWeapon* LeftWeapon;

};
