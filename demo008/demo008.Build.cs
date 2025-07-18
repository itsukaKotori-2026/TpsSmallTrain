// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class demo008 : ModuleRules
{
	public demo008(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
