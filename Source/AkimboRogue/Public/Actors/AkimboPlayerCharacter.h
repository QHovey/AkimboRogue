// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// Engine Includes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

// Project Includes
#include "Attributes/AkimboAttributeSet.h"
#include "AbilitySystem/AkimboGameplayAbility.h"
#include "Actors/AkimboWeapon.h"
#include "Actors/AkimboCharacter.h"

#include "AkimboPlayerCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)
class AAkimboPlayerCharacter : public AAkimboCharacter
{
	GENERATED_BODY()

protected:

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* VRRoot;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VR, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

	/** Position of the right hand when in FirstPerson View. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirstPerson)
	class USceneComponent* R_FPHandPos;

	/** Position of the left hand when in FirstPerson View. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirstPerson)
	class USceneComponent* L_FPHandPos;

public:
	AAkimboPlayerCharacter();

protected:

	virtual void BeginPlay();
	virtual void PossessedBy(AController* NewController);
	virtual void OnConstruction(const FTransform& Transform) override;

public:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
	uint32 bUsingMotionControllers : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = VR)
	bool IsVR;

	UFUNCTION(BlueprintImplementableEvent)
	void OnVRModeChanged(bool VREnabled);

protected:

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Toggles VRMode On/Off. VR Mode connects hands to MotionControllers and Camera to Headset. 
	Non-VR (FirstPerson) Mode connects hands to static locations in fron of camera and connects camera to Mouse / Keyboard. */
	void ToggleVRMode();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }

};

