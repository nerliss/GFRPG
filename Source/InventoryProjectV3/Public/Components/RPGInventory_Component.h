// Destruction Games. 2022

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 SlotsNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 Gold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlot> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FAnchors Anchors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FVector2D Alignment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClassTypes")
	TSubclassOf<URPGInventory_Window_Widget> InventoryWindowWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	URPGInventory_Window_Widget* InventoryWindowWidget;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PrepareInventory();

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool AddToInventory(FInventorySlot ContentToAdd);

	bool CreateStack (FInventorySlot ContentToAdd);
	bool AddToStack (FInventorySlot ContentToAdd, int32 SlotIndex);
	bool HasPartialStack(FInventorySlot Slot, int32& OutSlotIndex);
	
};
