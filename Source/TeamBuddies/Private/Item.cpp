// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamBuddies.h"
#include "Item.h"

AItem::AItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Items are true when they are created
	bIsActive = true;

	// Create the root BoxComponent to handle the Item's collision
	BaseCollisionComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BaseBoxComponent"));

	// Set the BoxComponent as the root component
	RootComponent = BaseCollisionComponent;

	// Create the static mesh component
	ItemMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ItemMesh"));

	// Attach the StaticMeshComponent to the root component
	ItemMesh->AttachTo(RootComponent);

	// wip
	BaseCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::BeginOverlap);

	/** Spawn volume can't tick by default so this enables it */
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginOverlap(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AItem::BeginOverlap"));
	
	// Try to cast OtherActor to a ATeamBuddiesCharacter
	ATeamBuddiesCharacter* MyCharacter = Cast<ATeamBuddiesCharacter>(OtherActor);

	if (MyCharacter != NULL)
	{
		OnPlayerOverlap(*MyCharacter);
	}
}

// not sure if passing the variables correctly
void AItem::OnPlayerOverlap(ATeamBuddiesCharacter &MyCharacter)
{
	// todo respawning

	// destroy this Item
	Destroy();
}

/** Override the default tick function */
void AItem::Tick(float DeltaSeconds)
{
	// Rotate mesh on every tick
	ItemMesh->AddRelativeRotation(FRotator(0.f, 0.5f, 0.f), false);
}
