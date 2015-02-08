// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamBuddies.h"
#include "Health.h"

AHealth::AHealth(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Health Items give 50 health
	iAmount = 50;
}

void AHealth::OnPlayerOverlap(ATeamBuddiesCharacter &MyCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("AHealth::OnPlayerOverlap"));
	MyCharacter.AddHealth(iAmount);

	// destroy this Item
	Super::OnPlayerOverlap(MyCharacter);
}

