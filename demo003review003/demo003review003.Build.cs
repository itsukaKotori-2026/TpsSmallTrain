// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class demo003review003 : ModuleRules
{
	public demo003review003(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
