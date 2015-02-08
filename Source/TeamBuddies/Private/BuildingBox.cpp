// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamBuddies.h"
#include "BuildingBox.h"

ABuildingBox::ABuildingBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Create the root BoxComponent to handle the Item's collision
	BaseCollisionComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BaseBoxComponent"));

	// Set the BoxComponent as the root component
	RootComponent = BaseCollisionComponent;

	// Create the static mesh component
	BoxMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BoxMesh"));

	// Enable physics for the statis mesh
	BoxMesh->SetSimulatePhysics(true);

	// Attach the StaticMeshComponent to the root component
	BoxMesh->AttachTo(RootComponent);
}
