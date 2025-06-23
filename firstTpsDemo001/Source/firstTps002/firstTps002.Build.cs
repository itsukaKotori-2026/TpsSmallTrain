// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class firstTps002 : ModuleRules
{
	public firstTps002(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
