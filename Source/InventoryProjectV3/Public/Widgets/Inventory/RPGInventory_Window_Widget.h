// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGInventory_Window_Widget.generated.h"

class URPGInventory_Component;
class UAkAudioEvent;
class UTextBlock;
class UGridPanel;
class UButton;
class URPGInventory_Slot_Widget;

/**
 * Inventory window widget native class.
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGInventory_Window_Widget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	URPGInventory_Window_Widget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:

	void InitializeWindowWidget();

	UFUNCTION()
	void CloseWindow();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<URPGInventory_Slot_Widget> InventorySlotWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* InventoryTitle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* InventoryGrid;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonClose;

	// MyTODO: Move these sounds to a struct or something
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	UAkAudioEvent* OpenInventorySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	UAkAudioEvent* CloseInventorySound;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetGoldAmount() const;

public:

	UFUNCTION(BlueprintCallable)
	URPGInventory_Component* GetAssociatedInventory() const;

	void SetAssociatedInventory(URPGInventory_Component* NewInventory);

private:

	/* Inventory component to take data from. Set when created in URPGInventory_Component::ToggleInventory() */
	URPGInventory_Component* AssociatedInventory;
};
