// Destruction Games. 2022

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Engine/EngineTypes.h"
#include "RPGInventoryConfig.generated.h"

class ARPGItem_Base;

USTRUCT(Blueprintable, BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	bool bQuestItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	bool bStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item", meta = (EditCondition = "bStackable", EditConditionHides))
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	bool bConsumable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	UTexture2D* Thumbnail;	

	/** !!!UNUSED!!! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	float Durability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory item")
	TSubclassOf<ARPGItem_Base> Class;

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
		Quantity = 0;
	}
};
