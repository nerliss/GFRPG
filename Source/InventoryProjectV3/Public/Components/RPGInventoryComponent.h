// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widgets/Layout/Anchors.h"
#include "Items/RPGInventoryConfig.h"
#include "RPGInventoryComponent.generated.h"

class URPGInventoryWindowWidget;
class ARPGItemBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYPROJECTV3_API URPGInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URPGInventoryComponent();

	/* TODO: Review these function, encapsulate if required */
	/* Creates inventory window if none is present - destroys otherwise */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	/* Add specified item to the inventory. Creates new stack if there is room for it. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddToInventory(FInventorySlot ContentToAdd);

	/* Query specific item in the inventory. Usually used for quests or dialogues */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool QueryInventory(TSubclassOf<ARPGItemBase> QueryItem, int32 QueryAmount, int32& FirstItemInstance, int32& ItemsAmount) const;
	bool QueryInventory(TSubclassOf<ARPGItemBase> QueryItem, int32 QueryAmount) const;

	void RemoveItem(TSubclassOf<ARPGItemBase> ItemToRemove, int32 AmountToRemove);
	
#if !UE_BUILD_SHIPPING
	void DebugPrintInventory();
#endif

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ExpandInventory(const int32 NewSlotsNumber);

	/* Name of the inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FText Name;

	/* Quantity of slots in the inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 SlotsNumber;

	/* Amount of money */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 Gold;

	/* The inventory container itself. EditAnywhere to be able to change items in inventory per actor. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlot> Inventory;

	/* Inventory widget anchors */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FAnchors Anchors;

	/* Inventory widget alignment */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FVector2D Alignment;

	/* Inventory widget class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<URPGInventoryWindowWidget> InventoryWindowWidgetClass;

	/* Inventory widget instance reference */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	URPGInventoryWindowWidget* InventoryWindowWidget;

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Resizes inventory and fills the slots with data from Inventory array */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PrepareInventory();

private:

	bool CreateStack(FInventorySlot ContentToAdd);
	bool AddToStack (FInventorySlot ContentToAdd, int32 SlotIndex);
	bool HasPartialStack (FInventorySlot Slot, int32& OutSlotIndex);

};
