// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class demo006 : ModuleRules
{
	public demo006(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
