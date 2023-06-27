#pragma once
#include "CoreMinimal.h"

class FUniversalActorTweakerLogic
{
public:
	void SetClickedObject(AActor* InClickedObject);
	void OnPropertyChanged(const FPropertyChangedEvent& InPropertyChanged);

private:
	UWorld* GetWorld();
	void SetComponentName(const FString InComponentFullPath);
	void PopulateActorsArray(const UWorld* InContextWorld, TArray<TObjectPtr<AActor>>& OutArrayToPopulate) const;
	void GetValueFromComponent(const FPropertyChangedEvent& InPropertyChanged, void* InNewValue);
	void GetValueFromPrimitiveComponent(const FPropertyChangedEvent& InPropertyChanged, void* InNewValue) const;
	void SetValueInPrimitiveComponentContainer(const TObjectPtr<AActor> InActor, const FPropertyChangedEvent& InPropertyChanged, const void* InNewValue) const;
	void SetValueInObjectContainer(TObjectPtr<UObject> InContainer,const TObjectPtr<AActor> InActor, const FPropertyChangedEvent& InPropertyChanged, const void* InNewValue);

private:
	FString ComponentName;
	TObjectPtr<UObject> ClickedObject;
};
