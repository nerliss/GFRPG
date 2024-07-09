// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "RPG_CheatManager.generated.h"

/**
 * RPG Cheat Manager
 */
UCLASS()
class INVENTORYPROJECTV3_API URPG_CheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UFUNCTION(Exec)
	void RPGDebug_ExpandInventory(const int32 NewSlotsNumber);

};
