// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Engine/EngineTypes.h"
#include "RPGInventoryConfig.generated.h"

class ARPGItem_Base;

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	ES_Head			UMETA(DisplayName = "Head"),
	ES_Shoulders	UMETA(DisplayName = "Shoulders"),
	ES_Chest		UMETA(DisplayName = "Chest"),
	ES_Hands		UMETA(DisplayName = "Hands"),
	ES_Belt			UMETA(DisplayName = "Belt"),
	ES_Legs			UMETA(DisplayName = "Legs"),
	ES_Feet			UMETA(DisplayName = "Feet"),
	ES_Finger		UMETA(DisplayName = "Finger"),
	ES_Neck			UMETA(DisplayName = "Neck"),
	ES_MainHand		UMETA(DisplayName = "MainHand"),
	ES_OffHand		UMETA(DisplayName = "OffHand"),
	ES_Max			UMETA(Hidden)
};

USTRUCT(Blueprintable, BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	FText Description;

	/* Is this item quest related? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	bool bQuestItem;

	/* MyTODO: Add quest reference / class this item is related to */

	/* Can the item be stack multiple times in one inventory slot? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	bool bStackable;

	/* How many items can we store in one inventory slot? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item", meta = (EditCondition = "bStackable", EditConditionHides))
	int32 MaxStackSize;

	/* Should the item be depleted upon usage? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	bool bConsumable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	UTexture2D* Thumbnail;	

	/** !!!UNUSED!!! */
	// MyTODO: Remove this
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	float Durability;

	/* Item's actor class, that actually has a model and where the use function is defined. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	TSubclassOf<ARPGItem_Base> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	EEquipmentSlot EquipmentSlot;

	FInventoryItem()
	{
		Name = FText::FromString("Item name");
		Description = FText::FromString("Item description. May be a very long text that contains a lot of symbols, perhaps ever color coded in future, or may be even a rich text.");
		bQuestItem = false;
		bStackable = true;
		MaxStackSize = 20;
		bConsumable = false;
		Thumbnail = nullptr;
		Durability = 10.f;
		Class = nullptr;
		EquipmentSlot = EEquipmentSlot::ES_Max;
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory slot")
	FInventoryItem Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory slot")
	int32 Quantity;

	FInventorySlot()
	{
		Item = FInventoryItem();
		Quantity = 0; // Must always be 0 by default - otherwise all the inventory slots will be filled with inexistent items
	}
};

/* Struct used in containers. Filled in blueprints, then it automatically copies filled data to actual inventory of a container */
USTRUCT(Blueprintable, BlueprintType)
struct FContainerItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory slot")
	TSubclassOf<ARPGItem_Base> Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory slot")
	int32 Quantity;

	FContainerItem()
	{
		Item = nullptr;
		Quantity = 1;
	}
};
