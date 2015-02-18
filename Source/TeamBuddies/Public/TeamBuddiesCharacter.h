// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "BuildingBox.h"
#include "TeamBuddiesCharacter.generated.h"

UENUM(BlueprintType)
enum class ETeamBuddiesPlayerState : uint8
{
	EPlaying,
	EHasPickUp,
	EInVehicle
};


UCLASS(config=Game)
class ATeamBuddiesCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
public:
	ATeamBuddiesCharacter(const FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ATeamBuddiesProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** Health of our character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	float Health;

	// wip
	void AddHealth(uint64 amount);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class ABuildingBox* PickedUpBox;

	// todo: make private
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	ETeamBuddiesPlayerState CurrentState;
		
protected:

	/** Handler for interacting with objects, on 'E' press */
	UFUNCTION(BlueprintCallable, Category = Player)
	void OnInteract();

	bool DoInteractableTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void FindInteractable();

	/** Pick up a pickupable object */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void PickUpBox(ABuildingBox* MyBox);

	/** Pick up a pickupable object */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void DropBox();

	/** Handler for a touch input beginning. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Fires a projectile. */
	void OnFire();

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
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};