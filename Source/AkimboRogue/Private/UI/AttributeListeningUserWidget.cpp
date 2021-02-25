// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AttributeListeningUserWidget.h"

#include "AbilitySystemInterface.h"

#include "Attributes/AkimboAttributeSet.h"

void UAttributeListeningUserWidget::SetOwner(AActor* NewOwner)
{
	if (ensure(NewOwner->Implements<UAbilitySystemInterface>())) {
		Owner = NewOwner;
		AbilitySystemInterface = Cast<IAbilitySystemInterface>(Owner);

		UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();
		if (ASC) {
			ASC->GetGameplayAttributeValueChangeDelegate(UAkimboAttributeSet::GetHealthAttribute()).AddUObject(this, &UAttributeListeningUserWidget::OnHealthChange);
		}
	}
}
