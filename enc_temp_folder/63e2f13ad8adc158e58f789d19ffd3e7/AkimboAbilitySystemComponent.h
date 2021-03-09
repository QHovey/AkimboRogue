// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AkimboAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = AbilitySystem, hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class AKIMBOROGUE_API UAkimboAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	/** Bind to an input component with some default action names */
	virtual void BindToInputComponent(UInputComponent* InputComponent);

	/** Bind to an input component with customized bindings */
	virtual void BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo);

	// Set the parent ASC of this child ASC. Used to follow a chain of ASCs back to the original owner of the chain.
	void SetOwnerASC(UAkimboAbilitySystemComponent* NewOwner) { OwnerAbilitySystem = NewOwner; }
	
	// Follows the chain of ParentAbilityComponents and returns the root. May return itself.
	UAkimboAbilitySystemComponent* GetRootOwnerASC();

protected:

	// Chain ASCs together using this pointer
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAkimboAbilitySystemComponent> OwnerAbilitySystem;
};
