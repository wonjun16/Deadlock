// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Deadlock : ModuleRules
{
	public Deadlock(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AnimGraphRuntime", "Niagara", "UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        PublicIncludePaths.AddRange(new string[] { "Deadlock/UI", "Deadlock/GameMode" });
    }
}
