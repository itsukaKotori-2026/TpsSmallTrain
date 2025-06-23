// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class firstTps003 : ModuleRules
{
	public firstTps003(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
