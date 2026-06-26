// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/RPGInventoryConfig.h"
#include "RPGInventorySlotWidget.generated.h"

class URPGInventoryComponent;
class UImage;
class UTextBlock;
class URPGInventoryTooltipWidget;
class ARPGPlayerCharacter;
class ARPGPlayerController;

/**
 * Inventory slot widget native class.
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	URPGInventorySlotWidget(const FObjectInitializer& ObjectInitializer);

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
	URPGInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<URPGInventoryTooltipWidget> TooltipClass;

	UPROPERTY(BlueprintReadOnly)
	URPGInventoryTooltipWidget* TooltipReference;

	/* TODO: Optimize code without using these two variables */
	UPROPERTY(BlueprintReadOnly)
	ARPGPlayerCharacter* PlayerCharacterOwner;

	UPROPERTY(BlueprintReadOnly)
	ARPGPlayerController* PlayerControllerOwner;

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
