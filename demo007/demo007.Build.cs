// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class demo007 : ModuleRules
{
	public demo007(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
