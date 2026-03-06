// Copyright (c) Empire of Ash. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EmpireOfAshEditorTarget : TargetRules
{
    public EmpireOfAshEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5Latest;
        ExtraModuleNames.Add("EmpireOfAsh");
    }
}
