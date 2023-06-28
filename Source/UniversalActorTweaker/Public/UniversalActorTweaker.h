// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMenuBuilder;
class FToolBarBuilder;
class FUniversalActorTweakerLogic;

class FUniversalActorTweakerModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& InSpawnTabArgs);
	FName GetUniversalActorTweakerOutlinerFName();
	static FUniversalActorTweakerModule& Get()
	{
		static const FName ModuleName = "UniversalActorTweakerModule";
		return FModuleManager::LoadModuleChecked<FUniversalActorTweakerModule>(ModuleName);
	}

private:
	void RegisterMenus();

private:
	TSharedPtr<FUniversalActorTweakerLogic> UniversalTweakerLogic;
	TSharedPtr<FUICommandList> PluginCommands;
};
