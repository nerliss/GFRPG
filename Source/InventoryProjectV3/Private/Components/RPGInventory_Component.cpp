// Destruction Games. 2022


#include "Components/RPGInventory_Component.h"
#include "PlayerController/RPGPlayer_Controller.h"
#include "Items/RPGItem_Base.h"
#include "Widgets/Inventory/RPGInventory_Window_Widget.h"
#include "Widgets/RPGHUD_Widget.h"
#include "Utility/Utility.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Characters/RPGPlayerCharacter.h"

DEFINE_LOG_CATEGORY(LogRPGInventory);

URPGInventory_Component::URPGInventory_Component()
{
	PrimaryComponentTick.bCanEverTick = false;

	InventoryWindowWidget = nullptr;
	InventoryWindowWidgetClass = nullptr;

	Name.GetEmpty();
	SlotsNumber = 0;
	Anchors = 0.5f;
	Alignment = FVector2D(1.0f, 0.5f);
}

void URPGInventory_Component::BeginPlay()
{
	Super::BeginPlay();

	PrepareInventory();
}

void URPGInventory_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool URPGInventory_Component::AddToInventory(FInventorySlot ContentToAdd)
{
	// Check if the item is stackable
	if (ContentToAdd.Item.bStackable)
	{
		int32 StackSlotIndex = 0;

		// Check if there is an already existing stack
		if (HasPartialStack(ContentToAdd, StackSlotIndex))
		{
			// Add to it
			return AddToStack(ContentToAdd, StackSlotIndex);
		}
		else
		{
			// Create a new stack
			return CreateStack(ContentToAdd);
		}
	}
	
	// Create a new stack if the item is not stackable
	return CreateStack(ContentToAdd);
}

bool URPGInventory_Component::CreateStack(FInventorySlot ContentToAdd)
{
	bool bHasEmptySlot = false;
	int32 EmptySlotIndex = 0;

	// Find first empty slot
	for (int i = 0; i < Inventory.Num(); i++)
	{
		const FInventorySlot Slot = Inventory[i];

		if (Slot.Quantity <= 0)
		{
			bHasEmptySlot = true;
			EmptySlotIndex = i;

			UE_LOG(LogRPGInventory, Log, TEXT("[RPGInventory_Component::CreateStack] Found empty stack at %i position"), EmptySlotIndex);
			break;
		}
	}

	if (bHasEmptySlot)
	{
		Inventory[EmptySlotIndex] = ContentToAdd;
		
		UE_LOG(LogRPGInventory, Log, TEXT("[RPGInventory_Component::CreateStack] Stack created at %i position. Content added: item %s, quantity %i"),
		EmptySlotIndex, *ContentToAdd.Item.Name.ToString(), ContentToAdd.Quantity);
		return bHasEmptySlot;
	}
	
	UE_LOG(LogRPGInventory, Error, TEXT("[RPGInventory_Component::CreateStack] Inventory is full!"));
	return bHasEmptySlot;
}

bool URPGInventory_Component::AddToStack(FInventorySlot ContentToAdd, int32 SlotIndex)
{
	FInventorySlot ExistingStack = Inventory[SlotIndex];
	const int32 CurrentQuantity = ExistingStack.Quantity;

	// Check if potential quantity exceeds max stack size
	const bool bNeedToCreateAdditionalStack = (CurrentQuantity + ContentToAdd.Quantity) > ExistingStack.Item.MaxStackSize; 

	if (bNeedToCreateAdditionalStack)
	{
		UE_LOG(LogRPGInventory, Log, TEXT("[RPGInventory_Component::AddToStack] Additional stack created for %s"), *ContentToAdd.Item.Name.ToString());

		// Complete the existing stack
		ExistingStack.Item = ContentToAdd.Item;
		ExistingStack.Quantity = ContentToAdd.Item.MaxStackSize;

		// Find out how many items are left to add and create a new stack with this new quantity
		const int32 ItemsLeftToAdd = ContentToAdd.Quantity - (ContentToAdd.Item.MaxStackSize - CurrentQuantity);

		FInventorySlot TemporarySlot;
		TemporarySlot.Item = ContentToAdd.Item;
		TemporarySlot.Quantity = ItemsLeftToAdd;

		AddToInventory(TemporarySlot);

		// MyTODO: figure out when and why should this function return false
		return true;
	}
	
	// Add to existing stack
	ExistingStack.Quantity += ContentToAdd.Quantity;

	UE_LOG(LogRPGInventory, Log, TEXT("[RPGInventory_Component::AddToStack] Item %s added without creating an additional stack"), *ContentToAdd.Item.Name.ToString());
	return true;
}

bool URPGInventory_Component::HasPartialStack(FInventorySlot Slot, int32& OutSlotIndex)
{
	int ExistingStackSlotIndex = 0;
	bool bExistingStackFound = false;

	for (int i = 0; i < Inventory.Num(); i++)
	{
		const FInventorySlot& InventorySlot = Inventory[i];
		const bool bSameStackableItem = InventorySlot.Item.Class == Slot.Item.Class;
		const bool bExistingStackHasFreeSpace = InventorySlot.Quantity < Slot.Item.MaxStackSize;
		const bool bCanAddToExistingStack = bExistingStackHasFreeSpace && bSameStackableItem;

		if (bCanAddToExistingStack)
		{
			ExistingStackSlotIndex = i;
			bExistingStackFound = true;

			UE_LOG(LogRPGInventory, Log, TEXT("[RPGInventory_Component::HasPartialStack] Partial stack exists at position %i for slot %s and contains %i items"), 
			ExistingStackSlotIndex, *Slot.Item.Name.ToString(), InventorySlot.Quantity);
			break;
		}
	}

	OutSlotIndex = ExistingStackSlotIndex; // returns zero if no stack found
	return bExistingStackFound;
}

void URPGInventory_Component::PrepareInventory()
{
	Inventory.SetNum(SlotsNumber);

	for (int i = 0; i < Inventory.Num(); i++)
	{
		FInventorySlot Slot = Inventory[i];
		const TSubclassOf<ARPGItem_Base> ItemClass = Slot.Item.Class;
		if (!ItemClass)
		{
			// Item class is invalid
			continue;
		}

		const int32 Quantity = Slot.Quantity;

		const ARPGItem_Base* ItemBaseCDO = ItemClass.GetDefaultObject();
		if (!ItemBaseCDO)
		{
			// Item CDO is invalid
			continue;
		}

		Slot.Item = ItemBaseCDO->Item;
		Slot.Quantity = Quantity;
	}
}

void URPGInventory_Component::ToggleInventory()
{
	ARPGPlayer_Controller* PC = Cast<ARPGPlayer_Controller>(GetOwner()->GetInstigatorController());
	if (!PC)
	{
		// Somehow no player controller
		return;
	}

	// Check if the window is already open
	if (InventoryWindowWidget)
	{
		InventoryWindowWidget->RemoveFromParent();
		InventoryWindowWidget = nullptr;
		
		PC->bShowMouseCursor = false;
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);

		UE_LOG(LogRPGInventory, Warning, TEXT("[RPGInventory_Component::ToggleInventory] Inventory window destroyed."));
		return;
	}

	// Create inventory window
	if (!InventoryWindowWidgetClass)
	{
		UE_LOG(LogRPGInventory, Error, TEXT("[RPGInventory_Component::ToggleInventory] Inventory window widget class is not set!"));
		return;
	}

	InventoryWindowWidget = Cast<URPGInventory_Window_Widget>(CreateWidget(GetWorld(), InventoryWindowWidgetClass));
	if (!InventoryWindowWidget)
	{
		UE_LOG(LogRPGInventory, Error, TEXT("[RPGInventory_Component::ToggleInventory] Inventory window was not created!"));
		return;
	}

	PC->MainHUDWidget->HUDCanvas->AddChildToCanvas(InventoryWindowWidget);

	// This will probably get us the most recently-created slot (last slot)
	const int32 LastSlot = PC->MainHUDWidget->HUDCanvas->GetChildrenCount() - 1;

	UCanvasPanelSlot* InventoryWindowSlot = CastChecked<UCanvasPanelSlot>(PC->MainHUDWidget->HUDCanvas->GetSlots()[LastSlot]);

	// Setup inventory window slot
	InventoryWindowSlot->SetAutoSize(true);
	InventoryWindowSlot->SetAlignment(Alignment);
	InventoryWindowSlot->SetAnchors(Anchors);

	// Set input mode
	PC->bShowMouseCursor = true;
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC);

	UE_LOG(LogRPGInventory, Warning, TEXT("[RPGInventory_Component::ToggleInventory] Inventory window created."));
}
