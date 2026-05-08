// Oleksandr Tkachov 2021-2026

using UnrealBuildTool;
using System.Collections.Generic;

public class InventoryProjectV3Target : TargetRules
{
	public InventoryProjectV3Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "InventoryProjectV3" } );
	}
}
