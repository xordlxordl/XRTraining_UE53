// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XRTraining : ModuleRules
{
	public XRTraining(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput",
			"RtCore",
			"RtGame",
			"RtResourceModel",
			"RsGame",
            "RsGameCore",
            "AIModule",

        });
	}

}
