// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ViolentEndsTarget : TargetRules
{
	public ViolentEndsTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		// Folder structure
		ExtraModuleNames.AddRange( new string[] { 
			"ViolentEnds"
		});
	}
}
