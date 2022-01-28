// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CardGame2022EditorTarget : TargetRules
{
	public CardGame2022EditorTarget( TargetInfo Target) : base(Target)
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
