// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/RPGInventoryConfig.h"
#include "RPGInventory_Slot_Widget.generated.h"

class URPGInventory_Component;
class UImage;
class UTextBlock;
class URPGInventory_Tooltip_Widget;
class ARPGPlayerCharacter;
class ARPGPlayer_Controller;

/**
 * Inventory slot widget native class.
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGInventory_Slot_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	URPGInventory_Slot_Widget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ItemThumbnail;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemQuantity;

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;

	UPROPERTY(BlueprintReadOnly)
	FInventorySlot SlotContent;

	UPROPERTY(BlueprintReadOnly)
	URPGInventory_Component* InventoryReference;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URPGInventory_Tooltip_Widget> TooltipClass;

	UPROPERTY(BlueprintReadOnly)
	URPGInventory_Tooltip_Widget* TooltipReference;

	/* MyTODO: Optimize code without using these two variables */
	UPROPERTY(BlueprintReadOnly)
	ARPGPlayerCharacter* PlayerCharacterOwner;

	UPROPERTY(BlueprintReadOnly)
	ARPGPlayer_Controller* PlayerControllerOwner;
	/***********************************************************/

	UFUNCTION(BlueprintCallable)
	void UpdateThumbnail();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetItemQuantity() const;

	void RefreshSlot();

	bool UseItem();

private:

	void CreateTooltip(const FGeometry InGeometry, const FPointerEvent InMouseEvent);
	void RemoveTooltip();
};
