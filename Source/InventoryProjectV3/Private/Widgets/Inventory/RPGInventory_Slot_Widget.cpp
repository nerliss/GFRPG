// Destruction Games. 2022


#include "Widgets/Inventory/RPGInventory_Slot_Widget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Items/RPGItem_Base.h"
#include "Components/RPGInventory_Component.h"	
#include "Characters/RPGPlayerCharacter.h"
#include "PlayerController/RPGPlayer_Controller.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"

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

	// MyTODO: Destroy tooltip
}

void URPGInventory_Slot_Widget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	// MyTODO: Destroy tooltip
}

void URPGInventory_Slot_Widget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	// MyTODO: Create tooltip if the slot is not empty
}

FReply URPGInventory_Slot_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (InventoryReference == PlayerCharacterOwner->InventoryComp)
		{
			if (UseItem())
			{
				return FReply::Handled();
			}
		}
		else
		{
			// MyTODO: Move item to player's inventory
			return FReply::Handled();
		}
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return FReply::Handled();
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

FText URPGInventory_Slot_Widget::GetItemQuantity() const
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

void URPGInventory_Slot_Widget::RefreshSlot()
{
	SlotContent = InventoryReference->Inventory[SlotIndex];

	// Reset slot to defaults if empty
	if (SlotContent.Quantity <= 0)
	{
		SlotContent = FInventorySlot();
		InventoryReference->Inventory[SlotIndex] = SlotContent;

		// Just hide thumbnail when no items left (default image for slot background is stored elsewhere)
		ItemThumbnail->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
		return;
	}

	ItemThumbnail->SetBrushFromTexture(SlotContent.Item.Thumbnail);
}

bool URPGInventory_Slot_Widget::UseItem() 
{
	if (!SlotContent.Item.Class)
	{
		return false;
	}

	// Spawn item in the world to use
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);

	auto* ItemToUse = GetWorld()->SpawnActor<ARPGItem_Base>(SlotContent.Item.Class, SpawnTransform, SpawnParams);
	if (!ItemToUse)
	{
		return false;
	}

	const bool bUsedSuccessfuly = ItemToUse->UseItem();
	if (!bUsedSuccessfuly)
	{
		return false;
	}

	if (SlotContent.Item.bConsumable)
	{
		// Decrease item count if it is a consumable
		InventoryReference->Inventory[SlotIndex].Quantity--;

		// Refresh slot only for consumables now, let's see if we need to do it for any item
		RefreshSlot();

		const int NewQuantity = InventoryReference->Inventory[SlotIndex].Quantity;
		UE_LOG(LogTemp, Log, TEXT("Consumable item used. New quantity is %i"), NewQuantity);
	}

	ItemToUse->Destroy();

	// MyTODO: Update Quests here (NYI)
	return true;
}
