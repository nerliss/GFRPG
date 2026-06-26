// Oleksandr Tkachov 2021-2026


#include "Utility/RPGCheatManager.h"
#include "Components/RPGInventoryComponent.h"
#include "Utility/LogDefinitions.h"

void URPGCheatManager::RPGDebug_ExpandInventory(const int32 NewSlotsNumber)
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

	URPGInventoryComponent* InventoryComponent = PlayerPawn->FindComponentByClass<URPGInventoryComponent>();
	if (!InventoryComponent)
	{
		return;
	}

	UE_LOG(LogRPGCheats, Log, TEXT("Expanded Player's inventory to %d slots"), NewSlotsNumber);
	InventoryComponent->ExpandInventory(NewSlotsNumber);
}
