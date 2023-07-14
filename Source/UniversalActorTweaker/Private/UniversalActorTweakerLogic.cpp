#include "UniversalActorTweakerLogic.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "UniversalActorTweakerLogic"

void FUniversalActorTweakerLogic::SetClickedObject(AActor* InClickedObject)
{
	if (InClickedObject != nullptr)
	{
		ClickedObject = InClickedObject;
	}
}

void FUniversalActorTweakerLogic::OnPropertyChanged(const FPropertyChangedEvent& InPropertyChanged)
{
	TArray<TObjectPtr<AActor>> FoundActors;
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
	const EPropertyOwnerType OwnerType = GetValue(InPropertyChanged, ChangedClass, NewValue);

	if (NewValue != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("FUniversalActorTweakerSnapshot", "Property changed from Universal Tweaker"));
		for (AActor* FoundActor : FoundActors)
		{
			switch (OwnerType)
			{
				case EPropertyOwnerType::Actor:
					SetValueInObjectContainer(InPropertyChanged, FoundActor, NewValue);
					break;

				case EPropertyOwnerType::Component:
					{
						TSet<UActorComponent*> ActorComponents = FoundActor->GetComponents();
						for (UActorComponent* ActorComponent : ActorComponents)
						{
							FString ActorPath = ActorComponent->GetPathName();
							if (ActorPath.Contains(ComponentName))
							{
								SetValueInObjectContainer(InPropertyChanged, ActorComponent, NewValue);
							}
						}
					}
					break;

				case EPropertyOwnerType::PrimitiveComponent:
					SetValueInPrimitiveComponentContainer(InPropertyChanged, FoundActor, NewValue);
					break;

				case EPropertyOwnerType::None:
				default:
					return;
			}
		}
	}
}

UWorld* FUniversalActorTweakerLogic::GetWorld()
{
	if (GEditor)
	{
		for (const FWorldContext& Context : GEditor->GetWorldContexts())
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
	TArray<FString> SplitString;
	InComponentFullPath.ParseIntoArray(SplitString,TEXT("."), true);

	ComponentName = SplitString.Last();
}

void FUniversalActorTweakerLogic::PopulateActorsArray(const UWorld* InWorldContext, TArray<TObjectPtr<AActor>>& OutArrayToPopulate) const
{
	UClass* ClickedClass = ClickedObject.GetClass();
	if (ClickedClass != nullptr)
	{
		UGameplayStatics::GetAllActorsOfClass(InWorldContext, ClickedClass, OutArrayToPopulate);
	}
}

void FUniversalActorTweakerLogic::GetValueFromComponent(const FPropertyChangedEvent& InPropertyChanged, void* InNewValue)
{
	SetComponentName(InPropertyChanged.GetObjectBeingEdited(0)->GetPathName());

	for (const UActorComponent* ActorComponent : Cast<AActor>(ClickedObject)->GetComponents())
	{
		FString ActorPath = ActorComponent->GetPathName();
		if (ActorPath.Contains(ComponentName))
		{
			InPropertyChanged.Property->GetValue_InContainer(ActorComponent, InNewValue);
		}
	}
}

void FUniversalActorTweakerLogic::GetValueFromPrimitiveComponent(const FPropertyChangedEvent& InPropertyChanged, void* InNewValue) const
{
	UActorComponent* PrimitiveComponent = Cast<AActor>(ClickedObject)->GetComponentByClass(UPrimitiveComponent::StaticClass());

	if (IsValid(PrimitiveComponent))
	{
		const FBodyInstance* BodyInstance = Cast<UPrimitiveComponent>(PrimitiveComponent)->GetBodyInstance();
		if (BodyInstance != nullptr)
		{
			InPropertyChanged.Property->GetValue_InContainer(BodyInstance, InNewValue);
		}
	}
}
void FUniversalActorTweakerLogic::SetValueInPrimitiveComponentContainer(const FPropertyChangedEvent& InPropertyChanged, const TObjectPtr<AActor> InActor, const void* InNewValue) const
{
	UActorComponent* PrimitiveComponent = InActor->GetComponentByClass(UPrimitiveComponent::StaticClass());

	if (PrimitiveComponent != nullptr)
	{
		FBodyInstance* BodyInstance = Cast<UPrimitiveComponent>(PrimitiveComponent)->GetBodyInstance();
		if (BodyInstance != nullptr)
		{
			InPropertyChanged.Property->SetValue_InContainer(BodyInstance, InNewValue);
		}

	}
}

void FUniversalActorTweakerLogic::SetValueInObjectContainer(const FPropertyChangedEvent& InPropertyChanged, TObjectPtr<UObject> InContainer, const void* InNewValue)
{
	FProperty* FoundProperty = InContainer->GetClass()->FindPropertyByName(InPropertyChanged.GetPropertyName());
	if (FoundProperty != nullptr)
	{
		InContainer->Modify(true);
		InContainer->PreEditChange(FoundProperty);
		FoundProperty->SetValue_InContainer(InContainer, InNewValue);

		FPropertyChangedEvent Args(FoundProperty);
		Args.ChangeType = EPropertyChangeType::ValueSet;

		InContainer->PostEditChangeProperty(Args);
	}
}

EPropertyOwnerType FUniversalActorTweakerLogic::GetValue(const FPropertyChangedEvent& InPropertyChanged, const UClass* InClassToGetFrom, void* OutNewValue)
{
	if (InClassToGetFrom != nullptr)
	{
		if (InClassToGetFrom->IsChildOf(UActorComponent::StaticClass()))
		{
			GetValueFromComponent(InPropertyChanged, OutNewValue);
			return EPropertyOwnerType::Component;
		}
		if (InClassToGetFrom->IsChildOf(AActor::StaticClass()))
		{
			
			InPropertyChanged.Property->GetValue_InContainer(Cast<AActor>(ClickedObject), OutNewValue);
			return EPropertyOwnerType::Actor;
		}
	}
	else
	{
		GetValueFromPrimitiveComponent(InPropertyChanged, OutNewValue);
		return EPropertyOwnerType::PrimitiveComponent;
	}

	return EPropertyOwnerType::None;
}

#undef LOCTEXT_NAMESPACE
