// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UniversalActorTweakerStyle.h"

class FUniversalActorTweakerCommands : public TCommands<FUniversalActorTweakerCommands>
{
public:

	FUniversalActorTweakerCommands()
		: TCommands<FUniversalActorTweakerCommands>(TEXT("UniversalActorTweaker"), NSLOCTEXT("Contexts", "UniversalActorTweaker", "UniversalActorTweaker Plugin"), NAME_None, FUniversalActorTweakerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
