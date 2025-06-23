// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class firstTpsDemo002 : ModuleRules
{
	public firstTpsDemo002(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
