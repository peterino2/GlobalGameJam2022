// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EmbCore : ModuleRules
{
	public EmbCore(ReadOnlyTargetRules Target) : base(Target)
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
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
                "EmbNav",
                "EmbUtils",
                "EmbTd",
                "EmbAbility"
			}
        );
        PrivateDependencyModuleNames.AddRange(new string[] { });
        PublicIncludePaths.Add(ModuleDirectory);
    }
}

