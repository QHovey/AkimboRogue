#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitAbilityEnd.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitAbilityEndDelegate, UGameplayAbility*, EndedAbility);

class AActor;

/**
 *	Waits for the actor to activate another ability
 */
UCLASS()
class AKIMBOROGUE_API UAbilityTask_WaitAbilityEnd : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FWaitAbilityEndDelegate OnEnd;

	virtual void Activate() override;

	UFUNCTION()
	void OnAbilityEnded(UGameplayAbility* EndedAbility);

	/** Wait until a new ability (of the same or different type) is activated. Only input based abilities will be counted unless IncludeTriggeredAbilities is true. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitAbilityEnd* WaitAbilityEnd_Class(UGameplayAbility* OwningAbility, TSubclassOf<UGameplayAbility> AbilityClass);

	TSubclassOf<UGameplayAbility> AbilityClass;

protected:

	virtual void OnDestroy(bool AbilityEnded) override;

	FDelegateHandle OnAbilityEndDelegateHandle;
};
