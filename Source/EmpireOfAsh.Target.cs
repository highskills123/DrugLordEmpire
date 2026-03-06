// Copyright (c) Empire of Ash. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EmpireOfAshTarget : TargetRules
{
    public EmpireOfAshTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5Latest;

        ExtraModuleNames.Add("EmpireOfAsh");
    }
}
