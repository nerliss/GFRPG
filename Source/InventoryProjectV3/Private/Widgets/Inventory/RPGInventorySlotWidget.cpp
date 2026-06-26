// Oleksandr Tkachov 2021-2026


#include "Widgets/Inventory/RPGInventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Items/RPGItemBase.h"
#include "Components/RPGInventoryComponent.h"	
#include "Characters/RPGPlayerCharacter.h"
#include "PlayerController/RPGPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Utility/LogDefinitions.h"
#include "Widgets/Inventory/RPGInventoryTooltipWidget.h"
#include "Widgets/RPGHUDWidget.h"

URPGInventorySlotWidget::URPGInventorySlotWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SlotIndex = 255;
	SlotContent = FInventorySlot();
	InventoryReference = nullptr;

	TooltipClass = nullptr;
	TooltipReference = nullptr;

	PlayerCharacterOwner = nullptr;
	PlayerControllerOwner = nullptr;
}

void URPGInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateThumbnail();

	PlayerCharacterOwner = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	PlayerControllerOwner = Cast<ARPGPlayerController>(GetOwningPlayer());
}

void URPGInventorySlotWidget::NativeDestruct()
{
	Super::NativeDestruct();

	RemoveTooltip();
}

void URPGInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	RemoveTooltip();
}

void URPGInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	CreateTooltip(InGeometry, InMouseEvent);
}

FReply URPGInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// TODO: Collapse to functions and clean it up
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// Use item
		if (InventoryReference == PlayerCharacterOwner->GetInventoryComponent())
		{
			if (UseItem())
			{
				return FReply::Handled();
			}
		}
		// Move item from container to player's inventory 
		else
		{
			if (SlotContent.Quantity > 0 && PlayerCharacterOwner->GetInventoryComponent()->AddToInventory(SlotContent))
			{
				InventoryReference->Inventory[SlotIndex] = FInventorySlot();
				RefreshSlot();

				return FReply::Handled();
			}
		}
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return FReply::Unhandled();
}

void URPGInventorySlotWidget::UpdateThumbnail()
{
	if (SlotContent.Quantity <= 0) // TODO: Check this
	{
		return;
	}

	// Set item thumbnail
	if (ItemThumbnail)
	{
		ItemThumbnail->SetBrushFromTexture(SlotContent.Item.Thumbnail);
		ItemThumbnail->SetColorAndOpacity(FLinearColor::White);
	}
}

FText URPGInventorySlotWidget::GetItemQuantity() const
{
	if (!IsValid(SlotContent.Item.Class))
	{
		return FText::FromString(TEXT(""));
	}

	// Return the quantity if the item is stackable
	return SlotContent.Item.bStackable ?
	FText::FromString(FString::FromInt(SlotContent.Quantity)) : 
	FText::FromString(TEXT(""));
}

void URPGInventorySlotWidget::RefreshSlot()
{
	SlotContent = InventoryReference->Inventory[SlotIndex];

	// Reset slot to defaults if empty
	if (SlotContent.Quantity <= 0)
	{
		SlotContent = FInventorySlot();
		InventoryReference->Inventory[SlotIndex] = SlotContent;

		// Just hide thumbnail when no items left (default image for slot background is stored elsewhere)
		ItemThumbnail->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));

		RemoveTooltip();
		return;
	}

	ItemThumbnail->SetBrushFromTexture(SlotContent.Item.Thumbnail);
}

bool URPGInventorySlotWidget::UseItem() 
{
	if (!SlotContent.Item.Class)
	{
		return false;
	}

	// Spawn item in the world to use
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::One());

	auto* ItemToUse = GetWorld()->SpawnActor<ARPGItemBase>(SlotContent.Item.Class, SpawnTransform, SpawnParams);
	if (!ItemToUse)
	{
		return false;
	}

	const bool bUsedSuccessfully = ItemToUse->UseItem();
	if (!bUsedSuccessfully)
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
		UE_LOG(LogRPGInventory, Verbose, TEXT("Consumable item used. New quantity is %i"), NewQuantity);
	}

	ItemToUse->Destroy();

	// MyTODO: Update Quests here (NYI)
	return true;
}

void URPGInventorySlotWidget::CreateTooltip(const FGeometry InGeometry, const FPointerEvent InMouseEvent)
{
	if (!TooltipClass)
	{
		UE_LOG(LogRPGInventory, Error, TEXT("[URPGInventorySlotWidget::CreateTooltip] TooltipClass is not set!"));
		return;
	}

	if (SlotContent.Quantity <= 0)
	{
		return;
	}

	TooltipReference = Cast<URPGInventoryTooltipWidget>(CreateWidget(GetWorld(), TooltipClass));
	if (!TooltipReference)
	{
		return;
	}

	TooltipReference->ItemName = SlotContent.Item.Name;
	TooltipReference->ItemDescription = SlotContent.Item.Description;
	TooltipReference->ItemThumbnailToSet = SlotContent.Item.Thumbnail;

	auto* PlayerController = GetOwningPlayer<ARPGPlayerController>();
	if (!PlayerController)
	{
		return;
	}

	auto* CanvasSlot = PlayerController->GetHUDWidget()->HUDCanvas->AddChildToCanvas(TooltipReference);
	if (!CanvasSlot)
	{
		return;
	}

	const FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
	const FVector2D TooltipPosition = MousePosition - InGeometry.GetLocalSize();

	CanvasSlot->SetAnchors(FAnchors());
	CanvasSlot->SetAlignment(FVector2D(1.f, 1.f));
	CanvasSlot->SetAutoSize(true);
	CanvasSlot->SetPosition(TooltipPosition);
}

void URPGInventorySlotWidget::RemoveTooltip()
{
	if (!TooltipReference)
	{
		return;
	}

	TooltipReference->RemoveFromParent();
	TooltipReference = nullptr;
}
