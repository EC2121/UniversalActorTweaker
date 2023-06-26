// Copyright Epic Games, Inc. All Rights Reserved.
#include "UniversalActorTweaker.h"

#include "Slate/SUniversalActorTweakerWindow.h"
#include "ToolMenus.h"
#include "UniversalActorTweakerCommands.h"
#include "UniversalActorTweakerStyle.h"

static const FName UniversalActorTweakerTabName("UniversalActorTweaker");

#define LOCTEXT_NAMESPACE "FUniversalActorTweakerModule"

void FUniversalActorTweakerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FUniversalActorTweakerStyle::Initialize();
	FUniversalActorTweakerStyle::ReloadTextures();

	FUniversalActorTweakerCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUniversalActorTweakerCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FUniversalActorTweakerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUniversalActorTweakerModule::RegisterMenus));
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(UniversalActorTweakerTabName,
	                                                  FOnSpawnTab::CreateRaw(
		                                                  this, &FUniversalActorTweakerModule::OnSpawnTab));
}

void FUniversalActorTweakerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FUniversalActorTweakerStyle::Shutdown();

	FUniversalActorTweakerCommands::Unregister();
}

void FUniversalActorTweakerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(UniversalActorTweakerTabName);
}

TSharedRef<SDockTab> FUniversalActorTweakerModule::OnSpawnTab(const FSpawnTabArgs& InSpawnTabArgs)
{
	TSharedRef<SDockTab> DockTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab);
	UniversalTweakerLogic = MakeShared<FUniversalActorTweakerLogic>();
	DockTab->SetContent(SNew(SUniversalActorTweakerWindow, UniversalTweakerLogic.ToSharedRef()));
	return DockTab;
}

FName FUniversalActorTweakerModule::GetUniversalActorTweakerOutlinerFName()
{
	return TEXT("UniversalActorTweakerOutliner");
}

void FUniversalActorTweakerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FUniversalActorTweakerCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitToolBarButton(FUniversalActorTweakerCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUniversalActorTweakerModule, UniversalActorTweaker)
