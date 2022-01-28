// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CardGame2022Target : TargetRules
{
	public CardGame2022Target( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[]
		{
			"EmbCore", 
			"EmbNav", 
			"EmbUtils", 
			"EmbAbility", 
			"EmbTd"
		} );
	}
}
