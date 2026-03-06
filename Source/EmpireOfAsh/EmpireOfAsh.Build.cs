// Copyright (c) Empire of Ash. All Rights Reserved.

using UnrealBuildTool;

public class EmpireOfAsh : ModuleRules
{
    public EmpireOfAsh(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "GameplayTasks",
            "AIModule",
            "NavigationSystem",
            "UMG",
            "Slate",
            "SlateCore",
            "NetCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "OnlineSubsystemNull"
        });

        // Mobile-specific
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("AndroidPermission");
        }
    }
}
