// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamBuddies.h"
#include "BoxSpawner.h"
#include "BuildingBox.h"

ABoxSpawner::ABoxSpawner(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/** Only spawn 3 boxes */
	MaxBoxes = 3;

	currentBoxCount = 0;

	/** Set the spawn delay range and get the first SpawnDelay */
	SpawnDelay = 1.0f;

	/** Spawn volume can't tick by default so this enables it */
	PrimaryActorTick.bCanEverTick = true;
}

void ABoxSpawner::SpawnBox()
{
	// check for a valid world
	UWorld* const World = GetWorld();

	if (World)
	{
		// UE_LOG(LogTemp, Warning, TEXT("ABoxSpawner::SpawnBox"));

		// set the spawn parameters
		FActorSpawnParameters SpawnParams;

		// defaults, spawn volume is owning it
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		// Get a random location to spawn at
		FVector SpawnLocation = GetActorLocation();

		// get a random rotation for the spawned item
		FRotator SpawnRotation;
		SpawnRotation.Yaw = FMath::FRand() * 360.f;
		SpawnRotation.Pitch = FMath::FRand() * 360.f;
		SpawnRotation.Roll = FMath::FRand() * 360.f;

		// spawn the pickup
		ABuildingBox* const Box = World->SpawnActor<ABuildingBox>(BoxToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

		currentBoxCount += 1;
	}
}


/** Override the default tick function */
void ABoxSpawner::Tick(float DeltaSeconds)
{
	// Always add delta time to our spawn time
	SpawnTime += DeltaSeconds;

	bool bShouldSpawn = ((SpawnTime > SpawnDelay) && (currentBoxCount < MaxBoxes));

	if (bShouldSpawn) {
		SpawnBox();

		// Deduct spawn delay from accumalted time value
		SpawnTime -= SpawnDelay;

		SpawnDelay = 1.f;	
	}
}
