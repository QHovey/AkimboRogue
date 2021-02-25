// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actors/FirstPersonCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"

#include "Attributes/AkimboAttributeSet.h"
#include "AbilitySystem/AkimboGameplayAbility.h"
#include "AbilitySystem/AkimboGameplayEffect.h"
#include "Actors/AkimboWeapon.h"
#include "AbilitySystem/AkimboAbilitySystemComponent.h"

#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFirstPersonTemplateCharacter

AFirstPersonCharacter::AFirstPersonCharacter()
{
	// Set size for collision capsule
	float initCapsuleHalfHeight = 96.0f;
	GetCapsuleComponent()->InitCapsuleSize(55.f, initCapsuleHalfHeight);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create the Root for the VR System. Should be on the bottom of the capsule, see OnConstuction() that ensures this.
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	VRRoot->SetupAttachment(GetCapsuleComponent());
	VRRoot->SetRelativeLocation(FVector(0.0f, 0.0f, -initCapsuleHalfHeight));

	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(VRRoot);
	CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(VRRoot);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(VRRoot);

	// Create First Person hand positions. Hands will attach to these when in Non-VR mode
	R_FPHandPos = CreateDefaultSubobject<USceneComponent>(TEXT("R_FPHandPos"));
	R_FPHandPos->SetupAttachment(CameraComponent);
	L_FPHandPos = CreateDefaultSubobject<USceneComponent>(TEXT("L_FPHandPos"));
	L_FPHandPos->SetupAttachment(CameraComponent);

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AFirstPersonCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	OnVRModeChanged(IsVR);
}

void AFirstPersonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Player will route gameplay effects to the weapons to keep them in sync via this delegate
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &AFirstPersonCharacter::OnApplyGameplayEffectToSelfCallback);
		AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &AFirstPersonCharacter::OnRemoveGameplayEffect);
	}
}

void AFirstPersonCharacter::OnConstruction(const FTransform& Transform)
{
	float CapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	VRRoot->SetRelativeLocation(FVector(0.f, 0.f, -CapsuleHalfHeight));
}

//////////////////////////////////////////////////////////////////////////
// AbilitySystem

void AFirstPersonCharacter::OnApplyGameplayEffectToSelfCallback(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	ApplyGameplayEffectToWeapon(RightWeapon, SpecApplied);
	ApplyGameplayEffectToWeapon(LeftWeapon, SpecApplied);
}

void AFirstPersonCharacter::OnRemoveGameplayEffect(const FActiveGameplayEffect& RemovedEffect)
{
	RemoveGameplayEffectFromWeapon(RightWeapon, RemovedEffect);
	RemoveGameplayEffectFromWeapon(LeftWeapon, RemovedEffect);
}

void AFirstPersonCharacter::ApplyGameplayEffectToWeapon(AAkimboWeapon* Weapon, const FGameplayEffectSpec& SpecApplied)
{
	if (Weapon)
	{
		UAkimboAbilitySystemComponent* ASC = Cast<UAkimboAbilitySystemComponent>(Weapon->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->ApplyGameplayEffectSpecToSelf(SpecApplied);
		}
	}
}

void AFirstPersonCharacter::RemoveGameplayEffectFromWeapon(AAkimboWeapon* Weapon, const FActiveGameplayEffect& SpecToRemove)
{
	if (Weapon)
	{
		UAkimboAbilitySystemComponent* ASC = Cast<UAkimboAbilitySystemComponent>(Weapon->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->RemoveActiveGameplayEffect(SpecToRemove.Handle);
		}
	}
}

void AFirstPersonCharacter::EquipRightWeapon(class AAkimboWeapon* InWeapon)
{
	RightWeapon = InWeapon;
	OnRightWeaponEquipped(RightWeapon);
}

void AFirstPersonCharacter::EquipLeftWeapon(class AAkimboWeapon* InWeapon)
{
	LeftWeapon = InWeapon;
	OnLeftWeaponEquipped(LeftWeapon);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFirstPersonCharacter::OnResetVR);
	PlayerInputComponent->BindAction("ToggleVR", IE_Pressed, this, &AFirstPersonCharacter::ToggleVRMode);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFirstPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFirstPersonCharacter::LookUpAtRate);

	// Attach the input to the ability system
	if (InputComponent && AbilitySystemComponent)
	{
		const FGameplayAbilityInputBinds InputBinds("Confirm", "Cancel", "EAkimboAbilityInputID");
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, InputBinds);
	}
}

//////////////////////////////////////////////////////////////////////////
// VR MODE

void AFirstPersonCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFirstPersonCharacter::ToggleVRMode()
{
	IsVR = !IsVR;

	OnVRModeChanged(IsVR);
}

//////////////////////////////////////////////////////////////////////////
// MOVEMENT

void AFirstPersonCharacter::MoveForward(float Value)
{
	if (!IsVR && Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFirstPersonCharacter::MoveRight(float Value)
{
	if (!IsVR && Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFirstPersonCharacter::TurnAtRate(float Rate)
{
	if (!IsVR)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AFirstPersonCharacter::LookUpAtRate(float Rate)
{
	if (!IsVR)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

