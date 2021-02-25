

#include "AbilityTasks/AbilityTask_WaitAbilityEnd.h"
#include "AbilitySystemComponent.h"

UAbilityTask_WaitAbilityEnd::UAbilityTask_WaitAbilityEnd(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UAbilityTask_WaitAbilityEnd* UAbilityTask_WaitAbilityEnd::WaitAbilityEnd_Class(UGameplayAbility* OwningAbility, TSubclassOf<UGameplayAbility> AbilityClass)
{
	UAbilityTask_WaitAbilityEnd* MyObj = NewAbilityTask<UAbilityTask_WaitAbilityEnd>(OwningAbility);
	MyObj->AbilityClass = AbilityClass;
	return MyObj;
}

void UAbilityTask_WaitAbilityEnd::Activate()
{
	if (AbilitySystemComponent)
	{
		OnAbilityEndDelegateHandle = AbilitySystemComponent->AbilityEndedCallbacks.AddUObject(this, &UAbilityTask_WaitAbilityEnd::OnAbilityEnded);
	}
}

void UAbilityTask_WaitAbilityEnd::OnAbilityEnded(UGameplayAbility* EndedAbility)
{
	if (EndedAbility->GetClass() == AbilityClass) {
		OnEnd.Broadcast(EndedAbility);
	}

	EndTask();
}


void UAbilityTask_WaitAbilityEnd::OnDestroy(bool AbilityEnded)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityActivatedCallbacks.Remove(OnAbilityEndDelegateHandle);
	}

	Super::OnDestroy(AbilityEnded);
}
