// Destruction Games. 2022


#include "Widgets/Inventory/RPGInventory_Slot_Widget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Items/RPGItem_Base.h"
#include "Characters/RPGPlayerCharacter.h"
#include "PlayerController/RPGPlayer_Controller.h"

URPGInventory_Slot_Widget::URPGInventory_Slot_Widget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SlotIndex = 255;
	SlotContent = FInventorySlot();
	InventoryReference = nullptr;

	PlayerCharacterOwner = nullptr;
	PlayerControllerOwner = nullptr;
}

void URPGInventory_Slot_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateThumbnail();

	PlayerCharacterOwner = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	PlayerControllerOwner = Cast<ARPGPlayer_Controller>(GetOwningPlayer());
}

void URPGInventory_Slot_Widget::NativeDestruct()
{
	Super::NativeDestruct();
}

void URPGInventory_Slot_Widget::UpdateThumbnail()
{
	if (SlotContent.Quantity <= 0) // MyTODO: Check this
	{
		// Slot is empty
		return;
	}

	// Set item thumbnail
	if (ItemThumbnail)
	{
		ItemThumbnail->SetBrushFromTexture(SlotContent.Item.Thumbnail);
		ItemThumbnail->SetColorAndOpacity(FLinearColor::White);
	}
}

FText URPGInventory_Slot_Widget::GetItemQuantity()
{
	if (!IsValid(SlotContent.Item.Class))
	{
		// Invalid item class
		return FText::FromString(TEXT(""));
	}

	// Return the quantity if the item is stackable
	return SlotContent.Item.bStackable ?
	FText::FromString(FString::FromInt(SlotContent.Quantity)) : 
	FText::FromString(TEXT(""));
}
