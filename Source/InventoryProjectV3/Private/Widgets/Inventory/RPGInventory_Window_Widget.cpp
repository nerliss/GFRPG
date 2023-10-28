// Oleksandr Tkachov 2022-2023


#include "Widgets/Inventory/RPGInventory_Window_Widget.h"
#include "Widgets/Inventory/RPGInventory_Slot_Widget.h"
#include "Components/RPGInventory_Component.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "AkGameplayStatics.h"
#include "GameFramework/InputSettings.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Utility/LogDefinitions.h"

URPGInventory_Window_Widget::URPGInventory_Window_Widget(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	InventorySlotWidgetClass = nullptr;

	AssociatedInventory = nullptr;

	OpenInventorySound = nullptr;
	CloseInventorySound = nullptr;
}

void URPGInventory_Window_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWindowWidget();

	if (ButtonClose)
	{
		ButtonClose->OnClicked.AddDynamic(this, &URPGInventory_Window_Widget::CloseWindow);
	}
}

void URPGInventory_Window_Widget::NativeDestruct()
{
	Super::NativeDestruct();

	UAkGameplayStatics::PostEvent(CloseInventorySound, nullptr, 0, FOnAkPostEventCallback());
}

FReply URPGInventory_Window_Widget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	TArray<FInputActionKeyMapping> KeyMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName("Inventory", KeyMappings);

	for (auto& Key : KeyMappings)
	{
		if (Key.Key == InKeyEvent.GetKey())
		{
			AssociatedInventory->ToggleInventory();
			break;
		}
	}

	return FReply::Handled();
}

FReply URPGInventory_Window_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return FReply::Handled();
}

void URPGInventory_Window_Widget::InitializeWindowWidget()
{
	if (!InventorySlotWidgetClass)
	{
		UE_LOG(LogRPGUIHUD, Error, TEXT("[URPGInventory_Window_Widget::InitializeWindowWidget] InventorySlotWidgetClass is not valid! Set it in RPGInventory_Window_Widget blueprint"));
		return;
	}

	if (!GetAssociatedInventory())
	{
		UE_LOG(LogRPGUIHUD, Error, TEXT("[URPGInventory_Window_Widget::InitializeWindowWidget] AssociatedInventory is not valid!"));
		return;
	}

	InventoryTitle->SetText(GetAssociatedInventory()->Name);

	InventoryGrid->ClearChildren();

	for (int i = 0; i < GetAssociatedInventory()->Inventory.Num(); i++)
	{
		auto* InventorySlot = Cast<URPGInventory_Slot_Widget>(CreateWidget(GetWorld(), InventorySlotWidgetClass));
		if (!InventorySlot)
		{
			UE_LOG(LogRPGUIHUD, Error, TEXT("[URPGInventory_Window_Widget::InitializeWindowWidget] InventorySlot was not created!"));
			continue;
		}

		// Update inventory slot with related data
		InventorySlot->SlotIndex = i;
		InventorySlot->SlotContent = GetAssociatedInventory()->Inventory[i];
		InventorySlot->InventoryReference = GetAssociatedInventory();

		InventoryGrid->AddChildToGrid(InventorySlot, i / 8, i % 8);
	}

	SetFocus();

	UAkGameplayStatics::PostEvent(OpenInventorySound, nullptr, 0, FOnAkPostEventCallback());
}

void URPGInventory_Window_Widget::CloseWindow()
{
	GetAssociatedInventory()->ToggleInventory();
}

URPGInventory_Component* URPGInventory_Window_Widget::GetAssociatedInventory() const
{
	return AssociatedInventory;
}

void URPGInventory_Window_Widget::SetAssociatedInventory(URPGInventory_Component* NewInventory)
{
	AssociatedInventory = NewInventory;
}
