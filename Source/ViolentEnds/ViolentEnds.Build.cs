// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ViolentEnds : ModuleRules
{
	public ViolentEnds(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "UMG",
			"GameplayTasks", "GameplayTags", "Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new[] {"AIModule", "NavigationSystem", "Slate", "SlateCore"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}