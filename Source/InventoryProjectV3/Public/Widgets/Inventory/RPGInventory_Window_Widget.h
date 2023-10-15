// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGInventory_Window_Widget.generated.h"

class URPGInventory_Component;
class UAkAudioEvent;
class UTextBlock;
class UGridPanel;
class URPGInventory_Slot_Widget;

/**
 * Inventory window widget native class.
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGInventory_Window_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	URPGInventory_Window_Widget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<URPGInventory_Slot_Widget> InventorySlotWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* InventoryTitle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* InventoryGrid;

	UPROPERTY(BlueprintReadOnly)
	URPGInventory_Component* PlayersInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	UAkAudioEvent* OpenInventorySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	UAkAudioEvent* CloseInventorySound;

};
