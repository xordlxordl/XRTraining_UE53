// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class RtResourceModel : ModuleRules
{
	public RtResourceModel(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Core", 
			"CoreUObject", 
			"Engine",
			"ProceduralMeshComponent",
			"Json",
            "JsonUtilities",
			"RenderCore",
			"RtCore",
			"UMG",
			"Niagara",
			"SlateCore",
			"MeshDescription",
			"RealTimeImport",
			"glTFRuntime",
			"glTFRuntimeFBX",
			"Effekseer"
        });

		//////////////////////////////////////////////////////////////////////////////////////////////
		// fbxsdk 외부 라이브러리 추가 
		//////////////////////////////////////////////////////////////////////////////////////////////

		string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/"));
		string IncludePath = ThirdPartyPath + "fbxsdk_2020.3.2/include/";
		string LibraryPath = ThirdPartyPath + "fbxsdk_2020.3.2/x64/";

		PublicIncludePaths.Add(IncludePath);

		PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "libfbxsdk-md.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "libxml2-md.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "zlib-md.lib"));

		//////////////////////////////////////////////////////////////////////////////////////////////
	}
}
