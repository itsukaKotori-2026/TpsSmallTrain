// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class demo003train005 : ModuleRules
{
	public demo003train005(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
