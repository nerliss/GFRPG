// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widgets/Layout/Anchors.h"
#include "Items/RPGInventoryConfig.h"
#include "RPGInventory_Component.generated.h"

class URPGInventory_Window_Widget;
class ARPGItem_Base;

DECLARE_LOG_CATEGORY_EXTERN(LogRPGInventory, Log, All);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYPROJECTV3_API URPGInventory_Component : public UActorComponent
{
	GENERATED_BODY()

public:	

	URPGInventory_Component();

	/* Name of the inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FText Name;

	/* Quantity of slots in the inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 SlotsNumber;

	/* Amount of money */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 Gold;

	/* The inventory container itself */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlot> Inventory;

	/* Inventory widget anchors */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FAnchors Anchors;

	/* Inventory widget alignment */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FVector2D Alignment;

	/* Inventory widget class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClassTypes")
	TSubclassOf<URPGInventory_Window_Widget> InventoryWindowWidgetClass;

	/* Inventory widget instance reference */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	URPGInventory_Window_Widget* InventoryWindowWidget;

	/* Creates inventory window if none is present - destroys otherwise */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

protected:

	virtual void BeginPlay() override;

	/* Resizes inventory and fills the slots with data from Inventory array */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PrepareInventory();

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool AddToInventory(FInventorySlot ContentToAdd);

	bool CreateStack (FInventorySlot ContentToAdd);
	bool AddToStack (FInventorySlot ContentToAdd, int32 SlotIndex);
	bool HasPartialStack (FInventorySlot Slot, int32& OutSlotIndex);
	
};
