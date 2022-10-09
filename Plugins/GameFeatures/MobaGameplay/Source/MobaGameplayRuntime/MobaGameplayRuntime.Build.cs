// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MobaGameplayRuntime : ModuleRules
{
    public MobaGameplayRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "LyraGame",
                "ModularGameplay",
                "ModularGameplayActors",
                "CommonGame",
                "SimpleThread",
                "Slate",
                "SlateCore",
                "InputCore",
                "GameplayMessageRuntime"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "GameplayTags",
                "GameplayTasks",
                "GameplayAbilities",
                "CommonUI",
                "UMG",
                "DataRegistry",
                "AsyncMixin",
                "EnhancedInput",
                "GameSubtitles",
                "DeveloperSettings",
                "AIModule",
                "LyraGame"
				// ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
