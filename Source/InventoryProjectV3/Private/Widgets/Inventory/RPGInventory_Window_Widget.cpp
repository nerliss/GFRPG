// Destruction Games. 2022


#include "Widgets/Inventory/RPGInventory_Window_Widget.h"
#include "Widgets/Inventory/RPGInventory_Slot_Widget.h"
#include "Components/RPGInventory_Component.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "AkGameplayStatics.h"

URPGInventory_Window_Widget::URPGInventory_Window_Widget(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	InventorySlotWidgetClass = nullptr;

	PlayersInventory = nullptr;

	OpenInventorySound = nullptr;
	CloseInventorySound = nullptr;
}

bool URPGInventory_Window_Widget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return false;
	}

	return true;
}

void URPGInventory_Window_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	UAkGameplayStatics::PostEvent(OpenInventorySound, nullptr, 0, FOnAkPostEventCallback());

	// Set a reference to player's inventory
	PlayersInventory = Cast<URPGInventory_Component>(GetOwningPlayer()->GetPawn()->GetComponentByClass(URPGInventory_Component::StaticClass()));

	if (!PlayersInventory)
	{
		UE_LOG(LogTemp, Error, TEXT("[RPGInventory_Window_Widget::NativeConstruct] PlayersInventory is not valid!"));
		return;
	}

	if (!InventorySlotWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[RPGInventory_Window_Widget::NativeConstruct] InventorySlotWidgetClass is not valid!"));
		return;
	}

	if (InventoryTitle)
	{
		InventoryTitle->SetText(PlayersInventory->Name);
	}

	for (int i = 0; i < PlayersInventory->Inventory.Num(); i++)
	{
		auto* InventorySlot = Cast<URPGInventory_Slot_Widget>(CreateWidget(GetWorld(), InventorySlotWidgetClass));
		if (!InventorySlot)
		{
			UE_LOG(LogTemp, Error, TEXT("[RPGInventory_Window_Widget::NativeConstruct] InventorySlot was not created!"));
			continue;
		}

		InventorySlot->SlotIndex = i;
		InventorySlot->SlotContent = PlayersInventory->Inventory[i];
		InventorySlot->InventoryReference = PlayersInventory;

		InventoryGrid->AddChildToGrid(InventorySlot, i/8, i%8);
	}
}

void URPGInventory_Window_Widget::NativeDestruct()
{
	Super::NativeDestruct();

	UAkGameplayStatics::PostEvent(CloseInventorySound, nullptr, 0, FOnAkPostEventCallback());
}
