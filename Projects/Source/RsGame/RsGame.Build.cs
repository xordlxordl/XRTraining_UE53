// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class RsGame : ModuleRules
{
	public RsGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Core", 
			"CoreUObject", 
			"Engine",
			"ProceduralMeshComponent",
            "EnhancedInput",
            "InputCore",
            "RtCore",
            "RtResourceModel",
            "UMG",
            "Slate",  //Loading Screen
            "SlateCore", //Loading Screen
            "PreLoadScreen", //Loading Screen
            "XmlParser",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
            "NavigationSystem",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "Json",
            "JsonUtilities",
            "HTTP",
            "HeadMountedDisplay",
			"Niagara"

		});
    }
}
