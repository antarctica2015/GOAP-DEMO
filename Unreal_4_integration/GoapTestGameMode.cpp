// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GoapTest.h"
#include "GoapTestGameMode.h"
#include "GoapTestPlayerController.h"
#include "GoapTestCharacter.h"

AGoapTestGameMode::AGoapTestGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = AGoapTestPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}