// Destruction Games. 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/RPGInventoryConfig.h"
#include "RPGInventory_Slot_Widget.generated.h"

class URPGInventory_Component;
class UImage;
class UTextBlock;
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

	/* MyTODO: Optimize code without using these two variables */
	UPROPERTY(BlueprintReadOnly)
	ARPGPlayerCharacter* PlayerCharacterOwner;

	UPROPERTY(BlueprintReadOnly)
	ARPGPlayer_Controller* PlayerControllerOwner;
	/***********************************************************/

	UFUNCTION(BlueprintCallable)
	void UpdateThumbnail();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetItemQuantity();

};
