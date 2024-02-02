// Oleksandr Tkachov 2022-2024

using UnrealBuildTool;
using System.Collections.Generic;

public class InventoryProjectV3Target : TargetRules
{
	public InventoryProjectV3Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "InventoryProjectV3" } );
	}
}
