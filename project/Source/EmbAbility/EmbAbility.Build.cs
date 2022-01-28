// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EmbAbility : ModuleRules
{
	public EmbAbility(ReadOnlyTargetRules Target) : base(Target)
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
                "EmbUtils",
                "EmbNav",
			}
        );
        PrivateDependencyModuleNames.AddRange(new string[] { });
        PublicIncludePaths.Add(ModuleDirectory);
    }
}
