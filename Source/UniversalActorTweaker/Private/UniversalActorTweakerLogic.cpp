#include "UniversalActorTweakerLogic.h"
#include "Kismet/GameplayStatics.h"

void FUniversalActorTweakerLogic::SetClickedObject(AActor* InClickedObject)
{
	if (InClickedObject != nullptr)
	{
		ClickedObject = InClickedObject;
	}
}


void FUniversalActorTweakerLogic::OnPropertyChanged(const FPropertyChangedEvent& InPropertyChanged)
{
	TArray<AActor*> FoundActors;
	const UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	PopulateActorsArray(World, FoundActors);
	const int32 NumberOfObjectsBeginEdited = InPropertyChanged.GetNumObjectsBeingEdited();
	if (NumberOfObjectsBeginEdited <= 0)
	{
		return;
	}
	void* NewValue = InPropertyChanged.Property->AllocateAndInitializeValue();

	const UClass* ChangedClass = InPropertyChanged.Property->GetOwnerClass();
	bool bIsComponent = false;
	bool bIsAnActor = false;
	bool bIsPrimitiveComponent = false;

	if (ChangedClass != nullptr)
	{
		if (ChangedClass->IsChildOf(UActorComponent::StaticClass()))
		{
			GetValueFromComponent(InPropertyChanged, NewValue);
			bIsComponent = true;
		}
		else if (ChangedClass->IsChildOf(AActor::StaticClass()))
		{
			InPropertyChanged.Property->GetValue_InContainer(Cast<AActor>(ClickedObject), NewValue);
			bIsAnActor = true;
		}
		else
		{
			return;
		}
	}
	else
	{
		GetValueFromPrimitive(InPropertyChanged, NewValue);
		bIsPrimitiveComponent = true;
	}


	if (NewValue != nullptr)
	{
		const FScopedTransaction Transaction(NSLOCTEXT("FUniversalActorTweakerLogic", "FUniversalActorTweakerSnapshot",
		                                               "UniversalActorTweakerUndo"));
		TObjectPtr<UObject> FoundContainer = nullptr;
		for (AActor* FoundActor : FoundActors)
		{
			if (bIsPrimitiveComponent)
			{
				SetValueInPrimitiveComponentContainer(FoundActor, InPropertyChanged, NewValue);
				continue;
			}
			FoundContainer = nullptr;
			if (bIsComponent)
			{
				TSet<UActorComponent*> ActorComponents = FoundActor->GetComponents();
				for (UActorComponent* ActorComponent : ActorComponents)
				{
					FString ActorPath = ActorComponent->GetPathName();
					if (ActorPath.Contains(ComponentName))
					{
						FoundContainer = ActorComponent;
						SetValueInObjectContainer(FoundContainer, FoundActor, InPropertyChanged, NewValue);
					}
				}
			}
			if (bIsAnActor)
			{
				FoundContainer = FoundActor;
				SetValueInObjectContainer(FoundContainer, FoundActor, InPropertyChanged, NewValue);
			}
		}
	}
}

UWorld* FUniversalActorTweakerLogic::GetWorld()
{
	if (GEditor)
	{
		for (const auto& Context : GEditor->GetWorldContexts())
		{
			if (Context.WorldType == EWorldType::Editor && Context.World())
			{
				return Context.World();
			}
		}
	}
	return nullptr;
}

void FUniversalActorTweakerLogic::SetComponentName(const FString InComponentFullPath)
{
	FString EditedComponentPath = InComponentFullPath;
	TArray<FString> SplitedString;
	InComponentFullPath.ParseIntoArray(SplitedString,TEXT("."), true);
	ComponentName = SplitedString.Last();
}

void FUniversalActorTweakerLogic::PopulateActorsArray(const UWorld* InWorldContext,
                                                      TArray<AActor*>& InArrayToPopulate) const
{
	UClass* ClickedClass = ClickedObject.GetClass();
	if (ClickedClass != nullptr)
	{
		UGameplayStatics::GetAllActorsOfClass(InWorldContext, ClickedClass, InArrayToPopulate);
	}
}

void FUniversalActorTweakerLogic::GetValueFromComponent(const FPropertyChangedEvent& InPropertyChanged,
                                                        void* InNewValue)
{
	SetComponentName(InPropertyChanged.GetObjectBeingEdited(0)->GetPathName());

	for (const UActorComponent* ActorComponent : Cast<AActor>(ClickedObject)->GetComponents())
	{
		FString ActorPath = ActorComponent->GetPathName();
		if (ActorPath.Contains(ComponentName))
		{
			InPropertyChanged.Property->GetValue_InContainer(
				ActorComponent,
				InNewValue);
		}
	}
}

void FUniversalActorTweakerLogic::GetValueFromPrimitive(const FPropertyChangedEvent& InPropertyChanged,
                                                        void* InNewValue) const
{
	UActorComponent* PrimitiveComponent = Cast<AActor>(ClickedObject)->GetComponentByClass(
		UPrimitiveComponent::StaticClass());
	if (PrimitiveComponent != nullptr)
	{
		FBodyInstance* BodyInstance = Cast<UPrimitiveComponent>(PrimitiveComponent)->GetBodyInstance();
		if (BodyInstance != nullptr)
		{
			InPropertyChanged.Property->GetValue_InContainer(
				BodyInstance,
				InNewValue);
		}
	}
}

void FUniversalActorTweakerLogic::SetValueInPrimitiveComponentContainer(const AActor* InActor,
                                                                        const FPropertyChangedEvent& InPropertyChanged,
                                                                        const void* InNewValue) const
{
	UActorComponent* PrimitiveComponent =
		InActor->GetComponentByClass(UPrimitiveComponent::StaticClass());
	if (PrimitiveComponent != nullptr)
	{
		FBodyInstance* BodyInstance = Cast<UPrimitiveComponent>(PrimitiveComponent)->GetBodyInstance();
		InPropertyChanged.Property->SetValue_InContainer(BodyInstance, InNewValue);
		UE_LOG(LogTemp, Log, TEXT("Succesfully changed %s property of %s Actor"),
		       *InPropertyChanged.Property->GetFName().ToString(),
		       *InActor->GetFName().ToString());
	}
}

void FUniversalActorTweakerLogic::SetValueInObjectContainer(TObjectPtr<UObject> InContainer, const AActor* InActor,
                                                            const FPropertyChangedEvent& InPropertyChanged,
                                                            const void* InNewValue)
{
	FProperty* FoundProperty = InContainer->GetClass()->FindPropertyByName(
		InPropertyChanged.GetPropertyName());
	if (FoundProperty != nullptr)
	{
		InContainer->Modify(true);
		InContainer->PreEditChange(FoundProperty);
		FoundProperty->SetValue_InContainer(InContainer, InNewValue);
		FPropertyChangedEvent Args(FoundProperty);
		Args.ChangeType = EPropertyChangeType::ValueSet;
		InContainer->PostEditChangeProperty(Args);
		UE_LOG(LogTemp, Log, TEXT("Succesfully changed %s property of %s Actor"),
		       *InPropertyChanged.Property->GetFName().ToString(),
		       *InActor->GetFName().ToString());
	}
}
