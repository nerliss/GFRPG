// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGInventoryWindowWidget.generated.h"

class URPGInventoryComponent;
class UAkAudioEvent;
class UTextBlock;
class UGridPanel;
class UButton;
class URPGInventorySlotWidget;

/**
 * Inventory window widget native class.
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGInventoryWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	URPGInventoryWindowWidget(const FObjectInitializer& ObjectInitializer);
	
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
	TSubclassOf<URPGInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* InventoryTitle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UGridPanel* InventoryGrid;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ButtonClose;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetGoldAmount() const;

public:

	UFUNCTION(BlueprintCallable)
	URPGInventoryComponent* GetAssociatedInventory() const;

	void SetAssociatedInventory(URPGInventoryComponent* NewInventory);

private:

	/* Inventory component to take data from. Set when created in URPGInventoryComponent::ToggleInventory() */
	URPGInventoryComponent* AssociatedInventory;
};
