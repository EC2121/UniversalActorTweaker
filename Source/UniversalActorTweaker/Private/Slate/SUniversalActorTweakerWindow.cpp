#include "Slate/SUniversalActorTweakerWindow.h"
#include "Kismet/GameplayStatics.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "SceneOutlinerModule.h"
#include "SceneOutlinerPublicTypes.h"
#include "Slate/Public/Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SScrollBox.h"

#define LOCTEXT_NAMESPACE "SUniversalActorTweakerWindow"

void SUniversalActorTweakerWindow::Construct(const FArguments& InArgs)
{
	UniversalActorTweakerLogic = MakeShared<FUniversalActorTweakerLogic>();
	
	FSceneOutlinerModule& SceneOutlinerModule = FModuleManager::LoadModuleChecked<
		FSceneOutlinerModule>("SceneOutliner");
	FPropertyEditorModule& EditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	FSceneOutlinerInitializationOptions InitOptions;
	InitOptions.OutlinerIdentifier = TEXT("UniversalActorTweakerOutliner");
	
	SceneOutliner = SceneOutlinerModule.CreateActorPicker(InitOptions, FOnActorPicked::CreateSP(this,
		                                                      &SUniversalActorTweakerWindow::OnActorPicked));
	
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bShowModifiedPropertiesOption = true;
	DetailsView = EditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->OnFinishedChangingProperties().AddRaw(UniversalActorTweakerLogic.Get(),
		&FUniversalActorTweakerLogic::OnPropertyChanged);
	ChildSlot
	[

		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SScrollBox)
				.Orientation(Orient_Vertical)
				.ScrollBarVisibility(EVisibility::Visible)
				.ScrollBarAlwaysVisible(true)
				+ SScrollBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SceneOutliner.ToSharedRef()
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1)
			[
				DetailsView.ToSharedRef()
			]
		]
	];
}

void SUniversalActorTweakerWindow::OnActorPicked(AActor* InActor)
{
	UniversalActorTweakerLogic->SetClickedObject(InActor);
	DetailsView->SetObject(InActor, true);
}

#undef LOCTEXT_NAMESPACE
