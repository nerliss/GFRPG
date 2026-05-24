// Oleksandr Tkachov 2021-2026


#include "Utility/RPG_CheatManager.h"
#include "Components/RPGInventory_Component.h"
#include "Utility/LogDefinitions.h"

void URPG_CheatManager::RPGDebug_ExpandInventory(const int32 NewSlotsNumber)
{
	const APlayerController* PlayerController = GetOuterAPlayerController();
	if (!PlayerController)
	{
		return;
	}

	const APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn)
	{
		return;
	}

	URPGInventory_Component* InventoryComponent = PlayerPawn->FindComponentByClass<URPGInventory_Component>();
	if (!InventoryComponent)
	{
		return;
	}

	UE_LOG(LogRPGCheats, Log, TEXT("Expanded Player's inventory to %d slots"), NewSlotsNumber);
	InventoryComponent->ExpandInventory(NewSlotsNumber);
}
