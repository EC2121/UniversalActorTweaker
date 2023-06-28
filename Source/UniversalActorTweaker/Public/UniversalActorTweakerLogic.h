#pragma once

#include "CoreMinimal.h"

enum class EPropertyOwnerType
{
	None,
	Actor,
	Component,
	PrimitiveComponent
};

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
	void SetValueInPrimitiveComponentContainer(const FPropertyChangedEvent& InPropertyChanged, const TObjectPtr<AActor> InActor, const void* InNewValue) const;
	void SetValueInObjectContainer(const FPropertyChangedEvent& InPropertyChanged, TObjectPtr<UObject> InContainer, const void* InNewValue);
	EPropertyOwnerType GetValue(const FPropertyChangedEvent& InPropertyChanged, const UClass* InClassToGetFrom, void* OutNewValue);

private:
	FString ComponentName;
	TObjectPtr<UObject> ClickedObject;
};
