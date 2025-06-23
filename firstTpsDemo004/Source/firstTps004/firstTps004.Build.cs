// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class firstTps004 : ModuleRules
{
	public firstTps004(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
