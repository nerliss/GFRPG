// Oleksandr Tkachov 2021-2026

using UnrealBuildTool;
using System.Collections.Generic;

public class InventoryProjectV3EditorTarget : TargetRules
{
	public InventoryProjectV3EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "InventoryProjectV3" } );
	}
}
