// Oleksandr Tkachov 2022-2024

using UnrealBuildTool;
using System.Collections.Generic;

public class InventoryProjectV3EditorTarget : TargetRules
{
	public InventoryProjectV3EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;

		ExtraModuleNames.AddRange( new string[] { "InventoryProjectV3" } );
	}
}
