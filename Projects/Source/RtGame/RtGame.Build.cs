// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class RtGame : ModuleRules
{
    public RtGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Engine Modules
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "EnhancedInput",
            "InputCore",
            "InputDevice",
            "UMG",
            "Slate",  //Loading Screen
            "SlateCore", //Loading Screen
            "PreLoadScreen", //Loading Screen
            "XmlParser",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "ProceduralMeshComponent",
            "AIModule",
            "NavigationSystem",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "Json",
            "JsonUtilities",
            "HTTP",
            "Networking",
            "Sockets",
            "Niagara",
            "RuntimeSpeechRecognizer", 
            "SignalProcessing",
            "AudioMixer",
            "AudioCapture",
            "Voice",
            "AudioExtensions",
            "XRBase",
            "HeadMountedDisplay",
            "OpenXRHMD",
            "OpenXRInput",
            "AudioCaptureCore"
            //"ZipUtility",
            //"SteamVRInput",
            //"SteamVRInputDevice",
        });

        PublicDependencyModuleNames.AddRange(new string[] {
            "OnlineSubsystem",
            "OnlineSubsystemUtils",

        });

        if (Target.bBuildEditor == true)
        {
            PublicDependencyModuleNames.AddRange(
                new string[] {
                    "UnrealEd",
                    "Kismet"
                }
            );  // @todo api: Only public because of WITH_EDITOR and UNREALED_API

            //CircularlyReferencedDependentModules.AddRange(
            //    new string[] {
            //        "UnrealEd",
            //        "Kismet"
            //    }
            //);
        }

        // PlguinModules
        PrivateDependencyModuleNames.AddRange(new string[] {
            "ViveOpenXRHMD",
            "ViveOpenXRWrapper",
            "ViveOpenXRPassthrough",
            "ViveOpenXRXrTracker",
            "ViveMocapKit",
            "RuntimeAudioImporter",
            "glTFRuntime",
            "glTFRuntimeFBX",
            "Effekseer",
        });

        String viveopenxrPath = "../Plugins/ViveOpenXR/Source/";
        PrivateIncludePaths.AddRange(new string[] {
            viveopenxrPath + "ViveOpenXRPassthrough/Private/External",
            viveopenxrPath + "ViveOpenXRPassthrough/Private",
        });
        

        // Modules
        PrivateDependencyModuleNames.AddRange(new string[] {
            "RtCore",
            "RtResourceModel",
        });

        ////////////////////////////////////////////////////////////////////////////////////////////////
        //// opus 외부 라이브러리 추가 
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/"));
        //string IncludePath = ThirdPartyPath + "Opus/include/";
        //string LibraryPath = ThirdPartyPath + "Opus/lib/";
        //
        //PublicIncludePaths.Add(IncludePath);
        //
        //PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "opus.lib"));

    }
}
