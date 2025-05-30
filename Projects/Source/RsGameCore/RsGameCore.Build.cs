// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class RsGameCore : ModuleRules
{
	public RsGameCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Core", 
			"CoreUObject", 
			"Engine",
			"ProceduralMeshComponent",
            "NavigationSystem",
        });

        PublicDependencyModuleNames.AddRange( new string[] {
            //"OnlineSubsystem", "OnlineSubsystemUtils"
        });
    }
}
