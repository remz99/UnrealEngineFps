// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TeamBuddiesCharacter.h"
#include "Item.generated.h"

/**
 * Base Class for Items in the game for example Health, Ammo. Items are "picked up"
 * when the player collides with them.
 */
UCLASS()
class TEAMBUDDIES_API AItem : public AActor
{
	GENERATED_BODY()

public:

	/** True when the Item can be "picked up" otherwise false */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	bool bIsActive;

	/** Ammout of each specific Item to give to the player, for example 50 Health. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	uint64 iAmount;
	
	/** Simple collision primitive to use as the root component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Item)
	class UBoxComponent* BaseCollisionComponent;

	/** StaticMeshComponent to represent the Item in the level */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Item)
	class UStaticMeshComponent* ItemMesh;

	/** Constructor */
	AItem(const class FObjectInitializer& ObjectInitializer);

	/** Begin Overlap function for Items*/
	UFUNCTION()
	void BeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	/** Function to handle player collisions, implemented by each subclass */
	virtual void OnPlayerOverlap(ATeamBuddiesCharacter &MyCharacter);

	/** Update on every frame */
	virtual void Tick(float DeltaSeconds) override;
};
