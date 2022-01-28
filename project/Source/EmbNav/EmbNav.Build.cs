
// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EmbNav : ModuleRules
{
	public EmbNav(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"NavigationSystem", 
                "EmbUtils",
			}
        );
        PrivateDependencyModuleNames.AddRange(new string[] { });
        PublicIncludePaths.Add(ModuleDirectory);
    }
}
