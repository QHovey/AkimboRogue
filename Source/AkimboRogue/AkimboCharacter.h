// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <AbilitySystemInterface.h>

#include "AkimboCharacter.generated.h"

UCLASS()
class AKIMBOROGUE_API AAkimboCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAkimboCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	/************************************************************************/
	/* Pawn Overrides														*/
	/************************************************************************/

	virtual void PossessedBy(AController* NewController) override;

	/************************************************************************/
	/* AbilitySystem Methods									            */
	/************************************************************************/

	// IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~IAbilitySystemInterface

	// Initialize the default attributes for this character
	virtual void InitializeAttributes();

	// Gives the default Abilities to this character
	virtual void GiveAbilities();

protected:

	/************************************************************************/
	/* AbilitySystem Properties									            */
	/************************************************************************/

	UPROPERTY(BlueprintReadWrite)
	class UAkimboAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UAkimboAttributeSet* Attributes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<class UAkimboGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<class UAkimboGameplayAbility>> DefaultAbilities;
};
