// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AkimboAbilitySystemComponent.generated.h"

class UAkimboGameplayAbility;

UENUM(BlueprintType)
enum class EAbilitySlot : uint8
{
	SLOT_ACTUATE			UMETA(DisplayName = "Actuate"),
	SLOT_ACTUATE_EFFECT		UMETA(DisplayName = "Actuate Effect"),
	SLOT_TELEPORT			UMETA(DisplayName = "Teleport"),
	SLOT_COUNT				UMETA(Hidden),
	SLOT_INVALID			UMETA(Hidden)
};

/**
 * 
 */
UCLASS(ClassGroup = AbilitySystem, hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class AKIMBOROGUE_API UAkimboAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	UAkimboAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

	/** Bind to an input component with some default action names */
	virtual void BindToInputComponent(UInputComponent* InputComponent);

	/** Bind to an input component with customized bindings */
	virtual void BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo);

	// Gives an ability to the ASC and assigns it to a slot for quick activation
	void GiveAndSlotAbility(const FGameplayAbilitySpec& Spec, EAbilitySlot Slot = EAbilitySlot::SLOT_INVALID);

	// Removes a slotted ability
	void RemoveSlottedAbility(EAbilitySlot Slot);

	// Set the owner/parent ASC of this child ASC. Used to follow a chain of ASCs back to the original owner of the chain.
	void SetOwnerASC(UAkimboAbilitySystemComponent* NewOwner) { OwnerAbilitySystem = NewOwner; }
	
	// Follows the chain of OwnerbilityComponents and returns the root. May return itself.
	UAkimboAbilitySystemComponent* GetRootOwnerASC();

public:

	// Akimbo specific TryActivate method. Be careful with ActiveAbility output param.
	// If Ability is NonInstanced, it'll be the CDO!
	// If Ability is InstancedPerActor, it'll be the single instance for this ASC
	// If Ability is InstancedPerExecution, it'll be the newly activated instance
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAkimboAbilityByClass(TSubclassOf<UAkimboGameplayAbility> Ability, UAkimboGameplayAbility*& ActiveAbility);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAkimboAbilityBySlot(EAbilitySlot Slot, UAkimboGameplayAbility*& ActiveAbility);

private:

	bool activateAkimboAbility(const FGameplayAbilitySpec& Spec, UAkimboGameplayAbility*& ActiveAbility);

protected:

	// Chain ASCs together using this pointer
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAkimboAbilitySystemComponent> OwnerAbilitySystem;

	TArray<FGameplayAbilitySpecHandle> SlottedAbilities;
};
