// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EmbEditorTarget : TargetRules
{
	public EmbEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { 
			"EmbCore", 
			"EmbNav", 
			"EmbUtils", 
			"EmbAbility", 
			"EmbTd" 
		} );
	}
}
