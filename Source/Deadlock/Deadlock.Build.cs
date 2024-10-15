// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Deadlock : ModuleRules
{
	public Deadlock(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AnimGraphRuntime" });
	}
}
