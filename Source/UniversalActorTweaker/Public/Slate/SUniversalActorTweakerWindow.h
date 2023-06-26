#pragma once

#include "Widgets/SCompoundWidget.h"
#include "UniversalActorTweakerLogic.h"
class IDetailsView;
class ISceneOutliner;

class SUniversalActorTweakerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SUniversalActorTweakerWindow){}
	SLATE_END_ARGS()
	void Construct(const FArguments& InArgs, TSharedRef<FUniversalActorTweakerLogic> InUniversalTweaker);
	void OnActorPicked(AActor* InActor) const;

private:
	TWeakPtr<FUniversalActorTweakerLogic> UniversalActorTweakerLogic;
	TSharedPtr<IDetailsView> DetailsView;
	TSharedPtr<ISceneOutliner> SceneOutliner;
};
