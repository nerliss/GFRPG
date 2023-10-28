// Oleksandr Tkachov 2022-2023

using UnrealBuildTool;
using System.Collections.Generic;

public class InventoryProjectV3EditorTarget : TargetRules
{
	public InventoryProjectV3EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "InventoryProjectV3" } );
	}
}
