// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"

#include "Blueprint/UserWidget.h"
#include "AttributeListeningUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AKIMBOROGUE_API UAttributeListeningUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void SetOwner(AActor* NewOwner);

    void OnHealthChange(const FOnAttributeChangeData& Data) { BP_OnHealthChange(Data.NewValue, Data.OldValue); }
    UFUNCTION(BlueprintImplementableEvent)
    void BP_OnHealthChange(float NewValue, float OldValue);

protected:
    // Owner of the widget. MUST IMPLEMENT IAbilitySystemInterface
    UPROPERTY(BlueprintReadOnly)
    class AActor* Owner;

    // Cached pointer to casted owner as IAbilitySystemInterface which was ensured.
    class IAbilitySystemInterface* AbilitySystemInterface;
};
