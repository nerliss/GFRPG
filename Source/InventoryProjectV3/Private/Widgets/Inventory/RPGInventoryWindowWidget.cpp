// Oleksandr Tkachov 2021-2026


#include "Widgets/Inventory/RPGInventoryWindowWidget.h"
#include "Widgets/Inventory/RPGInventorySlotWidget.h"
#include "Components/RPGInventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "GameFramework/InputSettings.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Utility/LogDefinitions.h"

URPGInventoryWindowWidget::URPGInventoryWindowWidget(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	InventorySlotWidgetClass = nullptr;

	AssociatedInventory = nullptr;
}

void URPGInventoryWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWindowWidget();

	if (ButtonClose)
	{
		ButtonClose->OnClicked.AddDynamic(this, &URPGInventoryWindowWidget::CloseWindow);
	}
}

void URPGInventoryWindowWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

FReply URPGInventoryWindowWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	TArray<FInputActionKeyMapping> KeyMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName("Inventory", KeyMappings);

	for (FInputActionKeyMapping& Key : KeyMappings)
	{
		if (Key.Key == InKeyEvent.GetKey())
		{
			AssociatedInventory->ToggleInventory();
			break;
		}
	}

	return FReply::Handled();
}

FReply URPGInventoryWindowWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return FReply::Handled();
}

void URPGInventoryWindowWidget::InitializeWindowWidget()
{
	if (!InventorySlotWidgetClass)
	{
		UE_LOG(LogRPGUIHUD, Error, TEXT("[URPGInventoryWindowWidget::InitializeWindowWidget] InventorySlotWidgetClass is not valid! Set it in RPGInventory_Window_Widget blueprint"));
		return;
	}

	if (!GetAssociatedInventory())
	{
		UE_LOG(LogRPGUIHUD, Error, TEXT("[URPGInventoryWindowWidget::InitializeWindowWidget] AssociatedInventory is not valid!"));
		return;
	}

	InventoryTitle->SetText(GetAssociatedInventory()->Name);

	InventoryGrid->ClearChildren();

	for (int i = 0; i < GetAssociatedInventory()->Inventory.Num(); i++)
	{
		auto* InventorySlot = Cast<URPGInventorySlotWidget>(CreateWidget(GetWorld(), InventorySlotWidgetClass));
		if (!InventorySlot)
		{
			UE_LOG(LogRPGUIHUD, Error, TEXT("[URPGInventoryWindowWidget::InitializeWindowWidget] InventorySlot was not created!"));
			continue;
		}

		// Update inventory slot with related data
		InventorySlot->SlotIndex = i;
		InventorySlot->SlotContent = GetAssociatedInventory()->Inventory[i];
		InventorySlot->InventoryReference = GetAssociatedInventory();

		InventoryGrid->AddChildToGrid(InventorySlot, i / 8, i % 8);
	}

	SetFocus();
}

void URPGInventoryWindowWidget::CloseWindow()
{
	GetAssociatedInventory()->ToggleInventory();
}

int32 URPGInventoryWindowWidget::GetGoldAmount() const
{
	return GetAssociatedInventory()->Gold;
}

URPGInventoryComponent* URPGInventoryWindowWidget::GetAssociatedInventory() const
{
	return AssociatedInventory;
}

void URPGInventoryWindowWidget::SetAssociatedInventory(URPGInventoryComponent* NewInventory)
{
	AssociatedInventory = NewInventory;
}
