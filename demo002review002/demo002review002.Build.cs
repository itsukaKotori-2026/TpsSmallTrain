// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class demo002review002 : ModuleRules
{
	public demo002review002(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
