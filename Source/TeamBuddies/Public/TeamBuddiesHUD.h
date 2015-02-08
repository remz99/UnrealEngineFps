// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "TeamBuddiesHUD.generated.h"

UCLASS()
class ATeamBuddiesHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATeamBuddiesHUD(const FObjectInitializer& ObjectInitializer);

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	/** variable for storing the font */
	UPROPERTY()
	UFont* HUDFont;

};

