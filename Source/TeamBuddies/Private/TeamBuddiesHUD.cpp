// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "TeamBuddies.h"
#include "TeamBuddiesHUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "TeamBuddiesCharacter.h"

ATeamBuddiesHUD::ATeamBuddiesHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshiarTexObj(TEXT("/Game/Textures/Crosshair"));
	CrosshairTex = CrosshiarTexObj.Object;

	// Set the font to the RobotoDistanceField font from the engine
	static ConstructorHelpers::FObjectFinder<UFont>HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = HUDFontOb.Object;
}


void ATeamBuddiesHUD::DrawHUD()
{
	// get the screen dimensions
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
										   (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)) );

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

	// draw the players health
	ATeamBuddiesCharacter* MyCharacter = Cast<ATeamBuddiesCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	FString HealthString = FString::Printf(TEXT("HEALTH:%10.1f"), FMath::Abs(MyCharacter->Health));
	DrawText(HealthString, FColor::White, Center.X - 425 , Center.Y + 400, HUDFont);
}

