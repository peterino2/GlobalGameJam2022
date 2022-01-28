// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EmbTd : ModuleRules
{
	public EmbTd(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"NavigationSystem", 
				"AIModule",
                "EmbUtils"
			}
        );
        PrivateDependencyModuleNames.AddRange(new string[] { });
        PublicIncludePaths.Add(ModuleDirectory);
    }
}

