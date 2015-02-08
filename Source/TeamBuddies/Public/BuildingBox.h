// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BuildingBox.generated.h"

/**
 * Boxes are able to be picked up and moved around by the player and 
 * can create weapons from when built from. If broken they can spawn an Item
 */
UCLASS()
class TEAMBUDDIES_API ABuildingBox : public AActor
{
	GENERATED_BODY()
	
public:

	/** Simple collision primitive to use as the root component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Item)
	class UBoxComponent* BaseCollisionComponent;

	/** StaticMeshComponent to represent the Item in the level */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Item)
	class UStaticMeshComponent* BoxMesh;

	/** Constructor */
	ABuildingBox(const class FObjectInitializer& ObjectInitializer);
};
