// Oleksandr Tkachov 2022-2024


#include "Utility/RPG_CheatManager.h"
#include "Components/RPGInventory_Component.h"

void URPG_CheatManager::RPGDebug_ExpandInventory(const int32 NewSlotsNumber)
{
	// Get the player controller
	const APlayerController* PlayerController = GetOuterAPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller not found!"));
		return;
	}

	// Get the player pawn
	const APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Player pawn not found!"));
		return;
	}

	// Get the inventory component
	URPGInventory_Component* InventoryComponent = PlayerPawn->FindComponentByClass<URPGInventory_Component>();
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory component not found!"));
		return;
	}

	InventoryComponent->ExpandInventory(NewSlotsNumber);
}
