// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FacePlayerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UFacePlayerComponent::UFacePlayerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFacePlayerComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UFacePlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector lookAtLoc = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FRotator rot = UKismetMathLibrary::FindLookAtRotation(this->GetComponentToWorld().GetLocation(), lookAtLoc);
	SetWorldRotation(rot);
}

