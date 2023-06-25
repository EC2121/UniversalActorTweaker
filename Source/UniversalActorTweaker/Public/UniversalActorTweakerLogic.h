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
	void PopulateActorsArray(const UWorld* InContextWorld, TArray<AActor*>& InArrayToPopulate) const;
	void GetValueFromComponent(const FPropertyChangedEvent& InPropertyChanged, void* InNewValue);
	void GetValueFromPrimitive(const FPropertyChangedEvent& InPropertyChanged, void* InNewValue) const;
	void SetValueInPrimitiveComponentContainer(const AActor* InActor, const FPropertyChangedEvent& InPropertyChanged, const void* InNewValue) const;
	void SetValueInObjectContainer(TObjectPtr<UObject> InContainer,const AActor* InActor, const FPropertyChangedEvent& InPropertyChanged, const void* InNewValue);
private:
	FString ComponentName;
	TObjectPtr<UObject> ClickedObject;
};
