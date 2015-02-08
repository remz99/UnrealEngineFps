// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "TeamBuddies.h"
#include "TeamBuddiesGameMode.h"
#include "TeamBuddiesHUD.h"
#include "TeamBuddiesCharacter.h"

ATeamBuddiesGameMode::ATeamBuddiesGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATeamBuddiesHUD::StaticClass();
}
