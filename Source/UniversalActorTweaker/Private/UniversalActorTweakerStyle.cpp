// Copyright Epic Games, Inc. All Rights Reserved.

#include "UniversalActorTweakerStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FUniversalActorTweakerStyle::StyleInstance = nullptr;

void FUniversalActorTweakerStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FUniversalActorTweakerStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FUniversalActorTweakerStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("UniversalActorTweakerStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FUniversalActorTweakerStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("UniversalActorTweakerStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("UniversalActorTweaker")->GetBaseDir() / TEXT("Resources"));

	Style->Set("UniversalActorTweaker.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FUniversalActorTweakerStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FUniversalActorTweakerStyle::Get()
{
	return *StyleInstance;
}
