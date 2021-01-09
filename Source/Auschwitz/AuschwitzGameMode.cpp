// Copyright Epic Games, Inc. All Rights Reserved.

#include "AuschwitzGameMode.h"
#include "AuschwitzHUD.h"
#include "AuschwitzCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAuschwitzGameMode::AAuschwitzGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAuschwitzHUD::StaticClass();
}
