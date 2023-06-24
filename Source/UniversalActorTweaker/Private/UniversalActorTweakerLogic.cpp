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
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	if (InPropertyChanged.Property != nullptr && ClickedObject != nullptr)
	{
		UClass* ClickedClass = ClickedObject.GetClass();
		if (ClickedClass != nullptr)
		{
			UGameplayStatics::GetAllActorsOfClass(World, ClickedClass, FoundActors);
		}
	}

	UClass* ChangedClass = InPropertyChanged.Property->GetOwnerClass();
	if (ChangedClass == nullptr)
	{
		return;
	}
	void* NewValue = InPropertyChanged.Property->AllocateAndInitializeValue();
	bool bIsClassComponent = false;
	bool bIsClassActor = false;
	const int32 NumberOfObjectsBeginEdited = InPropertyChanged.GetNumObjectsBeingEdited();
	if (NumberOfObjectsBeginEdited <= 0)
	{
		return;
	}

	if (ChangedClass->IsChildOf(UActorComponent::StaticClass()))
	{
		SetComponentName(InPropertyChanged.GetObjectBeingEdited(0)->GetPathName());

		for (UActorComponent* ActorComponent : Cast<AActor>(ClickedObject)->GetComponents())
		{
			FString ActorPath = ActorComponent->GetPathName();
			if (ActorPath.Contains(ComponentName))
			{
				InPropertyChanged.Property->GetValue_InContainer(
					ActorComponent,
					NewValue);
				bIsClassComponent = true;
			}
		}
	}
	else if (ChangedClass->IsChildOf(AActor::StaticClass()))
	{
		InPropertyChanged.Property->GetValue_InContainer(Cast<AActor>(ClickedObject), NewValue);
		bIsClassActor = true;
	}


	if (NewValue != nullptr)
	{
		const FScopedTransaction Transaction(NSLOCTEXT("FUniversalActorTweakerLogic", "FUniversalActorTweakerSnapshot",
		                                               "UniversalActorTweakerUndo"));
		TObjectPtr<UObject> FoundContainer = nullptr;
		for (AActor* FoundActor : FoundActors)
		{
			FoundContainer = nullptr;
			if (bIsClassComponent)
			{
				TSet<UActorComponent*> ActorComponents = FoundActor->GetComponents();
				for (UActorComponent* ActorComponent : ActorComponents)
				{
					FString ActorPath = ActorComponent->GetPathName();
					if (ActorPath.Contains(ComponentName))
					{
						FoundContainer = ActorComponent;
					}
				}
			}
			else if (bIsClassActor)
			{
				FoundContainer = FoundActor;
			}

			if (FoundContainer != nullptr)
			{
				FProperty* FoundProperty = FoundContainer->GetClass()->FindPropertyByName(
					InPropertyChanged.GetPropertyName());
				if (FoundProperty != nullptr)
				{
					FoundContainer->Modify(true);
					FoundContainer->PreEditChange(FoundProperty);
					FoundProperty->SetValue_InContainer(FoundContainer, NewValue);
					FPropertyChangedEvent Args(FoundProperty);
					Args.ChangeType = EPropertyChangeType::ValueSet;
					FoundContainer->PostEditChangeProperty(Args);
					UE_LOG(LogTemp, Log, TEXT("Succesfully changed %s property of %s Actor"),
					       *InPropertyChanged.Property->GetFName().ToString(),
					       *FoundActor->GetFName().ToString());
				}
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
				break;
			}
		}
	}
	return nullptr;
}

void FUniversalActorTweakerLogic::SetComponentName(const FString InComponentFullPath)
{
	FString EditedComponentPath = InComponentFullPath;
	TArray<FString> SpliitedString;
	InComponentFullPath.ParseIntoArray(SpliitedString,TEXT("."), true);
	ComponentName = SpliitedString.Last();
}
