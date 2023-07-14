# UniversalActorTweaker
 
## Intro
UniversalActorTweaker is a plugin that aims to expedite and simplify the process of making consistent modifications across multiple actors of the same class within a scene, With this plugin, you can easily modify a property of a single actor and have that change automatically applied to all other actors of the same class.
## Beta
UniversalActorTweaker is currently a work in progress. While the plugin offers features for modifying actor properties and propagating changes to all actors in the scene, it may still have some limitations and incomplete functionality.
## Nice to have
<details><summary> Save and load property configurations </summary><blockquote> 
This functionality would allow users to create presets or snapshots of modified actor properties and save them for later use
</blockquote></details>

## Installation
To install UniversalActorTweaker in your Unreal Engine 5 project, follow these steps:

1. Download the latest release of the plugin from the GitHub repository.
2. Copy the plugin folder (UniversalActorTweaker) into the Plugins directory of your Unreal Engine 5 project.
3. Launch the Unreal Engine editor.
4. Open your project.
5. Navigate to the Plugins section in the editor.
6. Locate UniversalActorTweaker in the list of plugins and make sure it is enabled.
7. Restart the Unreal Engine editor to apply the changes.

## Usage
1. Simply just open the widget using the button in the toolbar
<details><summary> Example Image </summary><blockquote>
	
![image](https://github.com/EC2121/UniversalActorTweaker/assets/89205380/d302c9c5-02ce-4c7a-a54d-a3176a69d0a5)
</blockquote></details>

2. Then select an actor and change one of its property
3. The property will be changed on all the actors of the same class!
<details><summary> Example Image </summary><blockquote>
	
![image](https://github.com/EC2121/UniversalActorTweaker/assets/89205380/6b9268df-08c3-41f3-8949-1aecc221baa8)
</blockquote></details>



## How it works

UniversalActorTweaker uses the FProperty system in Unreal Engine to enable property modification and customization as shown in this example.

```C++
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
 ```

## License

UniversalActorTweaker is released under the MIT License.

## Why
I know that i could have avoided the reflection system provided by Unreal Engine and achive the same results. However, i made the decision to utilize the reflection system as a means to deepen my understanding of Unreal Engine's architecture. By taking this approach, i aimed to develop a functional plugin and to expand my knowledge and expertise.

