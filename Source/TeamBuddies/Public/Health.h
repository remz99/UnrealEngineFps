// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "TeamBuddiesCharacter.h"
#include "Health.generated.h"

/**
 * Health Item Class
 */
UCLASS()
class TEAMBUDDIES_API AHealth : public AItem
{
	GENERATED_BODY()
	
public:

	/** Constructor */
	AHealth(const class FObjectInitializer& ObjectInitializer);

	/** Implementation of OnPlayerOverlap */
	virtual void OnPlayerOverlap(ATeamBuddiesCharacter &MyCharacter) override;
};
