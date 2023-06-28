#include "Slate/SUniversalActorTweakerWindow.h"
#include "Kismet/GameplayStatics.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "SceneOutlinerModule.h"
#include "SceneOutlinerPublicTypes.h"
#include "Slate/Public/Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SScrollBox.h"

#define LOCTEXT_NAMESPACE "SUniversalActorTweakerWindow"

void SUniversalActorTweakerWindow::Construct(const FArguments& InArgs, TSharedRef<FUniversalActorTweakerLogic> InUniversalTweaker)
{
	UniversalActorTweakerLogic = InUniversalTweaker;

	FSceneOutlinerModule& SceneOutlinerModule = FModuleManager::LoadModuleChecked<FSceneOutlinerModule>("SceneOutliner");
	FPropertyEditorModule& EditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FSceneOutlinerInitializationOptions InitOptions;
	InitOptions.OutlinerIdentifier = TEXT("UniversalActorTweakerOutliner");

	SceneOutliner = SceneOutlinerModule.CreateActorPicker(InitOptions, FOnActorPicked::CreateSP(this, &SUniversalActorTweakerWindow::OnActorPicked));

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;

	DetailsView = EditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->OnFinishedChangingProperties().AddSP(this, &SUniversalActorTweakerWindow::OnPropertyChanged);

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
				.ScrollBarAlwaysVisible(true)

				+ SScrollBox::Slot()
				.HAlign(HAlign_Center)
				[
					SceneOutliner.ToSharedRef()
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				DetailsView.ToSharedRef()
			]
		]
	];
}

void SUniversalActorTweakerWindow::OnActorPicked(AActor* InActor) const
{
	if (UniversalActorTweakerLogic.IsValid())
	{
		UniversalActorTweakerLogic.Pin()->SetClickedObject(InActor);
		DetailsView->SetObject(InActor, true);
	}
}

void SUniversalActorTweakerWindow::OnPropertyChanged(const FPropertyChangedEvent& InPropertyChanged) const
{
	if (UniversalActorTweakerLogic.IsValid())
	{
		UniversalActorTweakerLogic.Pin()->OnPropertyChanged(InPropertyChanged);
	}
}

#undef LOCTEXT_NAMESPACE
