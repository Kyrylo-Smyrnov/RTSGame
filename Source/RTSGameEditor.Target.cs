// https://github.com/Kyrylo-Smyrnov/RTSGame

using UnrealBuildTool;
using System.Collections.Generic;

public class RTSGameEditorTarget : TargetRules
{
	public RTSGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "RTSGame" } );
	}
}
