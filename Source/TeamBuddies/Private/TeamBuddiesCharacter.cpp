// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "TeamBuddies.h"
#include "TeamBuddiesCharacter.h"
#include "TeamBuddiesProjectile.h"
#include "Animation/AnimInstance.h"


//////////////////////////////////////////////////////////////////////////
// ATeamBuddiesCharacter

ATeamBuddiesCharacter::ATeamBuddiesCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->RelativeLocation = FVector(0.f, 0.f, -150.f);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Set the health on the player
	Health = 100;

	CurrentState = ETeamBuddiesPlayerState::EPlaying;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATeamBuddiesCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATeamBuddiesCharacter::OnFire);
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATeamBuddiesCharacter::TouchStarted);

	// custom event bindings
	// interact with objects
	InputComponent->BindAction("Interact", IE_Pressed, this, &ATeamBuddiesCharacter::OnInteract);

	InputComponent->BindAxis("MoveForward", this, &ATeamBuddiesCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATeamBuddiesCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ATeamBuddiesCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ATeamBuddiesCharacter::LookUpAtRate);
}

void ATeamBuddiesCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<ATeamBuddiesProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if(FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if(AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

}

void ATeamBuddiesCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// only fire for first finger down
	if (FingerIndex == 0)
	{
		OnFire();
	}
}

void ATeamBuddiesCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATeamBuddiesCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATeamBuddiesCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATeamBuddiesCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATeamBuddiesCharacter::AddHealth(uint64 Amount)
{
	Health += Amount;
}

/** Interact with objects in the world */
void ATeamBuddiesCharacter::OnInteract()
{
	switch (CurrentState)
	{
	case ETeamBuddiesPlayerState::EHasPickUp:
		DropBox();
		break;
	case ETeamBuddiesPlayerState::EInVehicle:
		break;
	case ETeamBuddiesPlayerState::EPlaying:
	default:
		FindInteractable();
		break;
	}
}

void ATeamBuddiesCharacter::FindInteractable_Implementation()
{
	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	
	if (DoInteractableTrace(&RV_Hit, &RV_TraceParams))
	{
		ABuildingBox* Box = Cast<ABuildingBox>(RV_Hit.GetActor());

		if (Box)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *RV_Hit.GetActor()->GetName());
			PickUpBox(Box);
		}
	}
}

bool ATeamBuddiesCharacter::DoInteractableTrace(FHitResult* RV_Hit, FCollisionQueryParams* RV_TraceParams)
{
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector Start = CameraLoc;
	FVector End = CameraLoc + (CameraRot.Vector() * 90.f);

	RV_TraceParams->bTraceComplex = true;
	RV_TraceParams->bTraceAsyncScene = true;
	RV_TraceParams->bReturnPhysicalMaterial = true;

	// line trace on the "Interactable" line channel from the center of the camera
	bool DidTrace = GetWorld()->LineTraceSingle(*RV_Hit, Start, End, ECC_EngineTraceChannel1, *RV_TraceParams);

	return DidTrace;
}

void ATeamBuddiesCharacter::PickUpBox_Implementation(ABuildingBox* MyBox)
{
	CurrentState = ETeamBuddiesPlayerState::EHasPickUp;
	PickedUpBox = MyBox;
	//MyBox->SetActorEnableCollision(false);

	// todo
	// attach actor to the building box placement component 
}

void ATeamBuddiesCharacter::DropBox_Implementation()
{
	ABuildingBox* Box = Cast<ABuildingBox>(PickedUpBox);

	if (Box)
	{	
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("drop box!"));
		
		//Box->SetActorEnableCollision(true);

		// todo
		// detach actor from component

		CurrentState = ETeamBuddiesPlayerState::EPlaying;
		PickedUpBox = NULL;		
	}
}