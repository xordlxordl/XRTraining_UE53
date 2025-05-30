// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class RtCore : ModuleRules
{
	public RtCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableExceptions = true;
		PrivateDependencyModuleNames.AddRange(new string[] {
			"Core", 
			"CoreUObject", 
			"Engine",
			"ProceduralMeshComponent",
            "EnhancedInput",
            "InputCore",
            "Json",
            "JsonUtilities",
			"EnhancedInput",
			"Niagara",
			"Slate",
			"SlateCore",
			"HTTP",
			"Networking",
			"Sockets",
			"UMG"
		//	"RtResourceModel"
		});

		
		////////////////////////////////////////////////////////////////////////////////////////////////
		// fbxsdk 외부 라이브러리 추가 
		//////////////////////////////////////////////////////////////////////////////////////////////
		
		string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/"));
		string IncludePath = ThirdPartyPath + "fbxsdk_2020.3.2/include/";
		string LibraryPath = ThirdPartyPath + "fbxsdk_2020.3.2/x64/";
		
		PublicIncludePaths.Add(IncludePath);
		
		PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "libfbxsdk-md.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "libxml2-md.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "zlib-md.lib"));
	}
}
