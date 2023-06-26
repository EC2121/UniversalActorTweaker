# UniversalActorTweaker
 
## Intro
This plugin allows you to select an actor in scene and change the paramaters of all the actor in scene of the same class


<details><summary> How to use  </summary><blockquote>
Simply just open the widget using the button in the toolbar

![image](https://github.com/EC2121/UniversalActorTweaker/assets/89205380/72af0128-c9a9-41b0-ac20-6ae5828410b0)

Then select an actor and change one of its property

![image](https://github.com/EC2121/UniversalActorTweaker/assets/89205380/6b9268df-08c3-41f3-8949-1aecc221baa8)


The property will be changed on all the actors of the same class!

![image](https://github.com/EC2121/UniversalActorTweaker/assets/89205380/e89c4a5a-8adf-440b-b17c-cc71e5259d5a)

</blockquote></details>
<details><summary> How it works </summary><blockquote>

The plugin works using the Unreal reflection system as shown in this example

```C++
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
	}
 ```


