// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AkimboGameplayAbility.h"
#include "AkimboWeapon.generated.h"

UENUM(BlueprintType)
enum class EHand : uint8
{
	NONE,
	LEFT,
	RIGHT
};

UCLASS()
class AKIMBOROGUE_API AAkimboWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAkimboWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void OnEquippedBy(APawn* Equipper, EHand EquippedToHand);

private:

	void InitializeAttributes();
	void GiveAbilities(EHand EquippedToHand);

protected:

	UPROPERTY(BlueprintReadWrite)
	class UAkimboAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UAkimboAttributeSet* Attributes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<class UAkimboGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<class UAkimboGameplayAbility>> DefaultAbilities;
};
