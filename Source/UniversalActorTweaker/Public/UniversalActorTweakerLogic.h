#pragma once
#include "CoreMinimal.h"

class FUniversalActorTweakerLogic
{
public:
	FUniversalActorTweakerLogic() = default;
public:
	void SetClickedObject(AActor* InClickedObject);
	void OnPropertyChanged(const FPropertyChangedEvent& InPropertyChanged);
private:
	UWorld* GetWorld();
	void SetComponentName(const FString InComponentFullPath);
private:
	FString ComponentName;
	TObjectPtr<UObject> ClickedObject;
};
