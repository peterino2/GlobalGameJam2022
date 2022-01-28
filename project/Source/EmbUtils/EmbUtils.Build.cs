// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EmbUtils : ModuleRules
{
	public EmbUtils(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core", 
				"CoreUObject", 
				"Engine", 
			}
        );
        PrivateDependencyModuleNames.AddRange(new string[] { });
        PublicIncludePaths.Add(ModuleDirectory);
    }
}
