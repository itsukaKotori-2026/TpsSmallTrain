// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class demo005 : ModuleRules
{
	public demo005(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
