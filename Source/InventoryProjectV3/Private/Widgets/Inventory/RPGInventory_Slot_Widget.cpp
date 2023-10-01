// Destruction Games. 2022


#include "Widgets/Inventory/RPGInventory_Slot_Widget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Items/RPGItem_Base.h"

URPGInventory_Slot_Widget::URPGInventory_Slot_Widget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SlotIndex = 255;
	SlotContent = FInventorySlot();
	InventoryReference = nullptr;
}

bool URPGInventory_Slot_Widget::Initialize()
{
	return Super::Initialize();
}

void URPGInventory_Slot_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateThumbnail();
}

void URPGInventory_Slot_Widget::NativeDestruct()
{
	Super::NativeDestruct();
}

void URPGInventory_Slot_Widget::UpdateThumbnail()
{
	//if (!IsValid(SlotContent))
	//{
	//	// Invalid slot
	//	return;
	//}

	if (SlotContent.Quantity <= 0)
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
	//if (!IsValid(SlotContent))
	//{
	//	// Invalid slot
	//	return FText::FromString(TEXT(""));
	//}

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
