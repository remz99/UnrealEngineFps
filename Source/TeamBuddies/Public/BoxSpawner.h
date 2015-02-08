// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BoxSpawner.generated.h"

/**
 * 
 */
UCLASS()
class TEAMBUDDIES_API ABoxSpawner : public AActor
{
	GENERATED_BODY()
	
public:

	ABoxSpawner(const class FObjectInitializer& ObjectInitializer);
	
	/** The Box to spawn */
	UPROPERTY(EditAnywhere, Category = Spawning)
	TSubclassOf<class ABuildingBox> BoxToSpawn;

	/** Maxmimum amount of boxes to spawn */
	UPROPERTY(EditAnywhere, Category = Spawning)
	float MaxBoxes;

	/** current box count */
	float currentBoxCount;

	/** Spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	float SpawnDelay;

	/** Update on every frame */
	virtual void Tick(float DeltaSeconds) override;

	/** Handles the spawning of a new pickup */
	void SpawnBox();

	/** The timer for when to spawn the pickup */
	float SpawnTime;
};
