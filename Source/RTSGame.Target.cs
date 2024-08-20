// https://github.com/Kyrylo-Smyrnov/RTSGame

using UnrealBuildTool;
using System.Collections.Generic;

public class RTSGameTarget : TargetRules
{
	public RTSGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "RTSGame" } );
	}
}
