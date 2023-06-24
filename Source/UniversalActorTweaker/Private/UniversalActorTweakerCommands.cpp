// Copyright Epic Games, Inc. All Rights Reserved.

#include "UniversalActorTweakerCommands.h"

#define LOCTEXT_NAMESPACE "FUniversalActorTweakerModule"

void FUniversalActorTweakerCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "UniversalActorTweaker", "Execute UniversalActorTweaker action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
