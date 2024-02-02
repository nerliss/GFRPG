// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGInventory_Tooltip_Widget.generated.h"

class UTexture2D;
class UImage;
class USizeBox;

/**
 * Inventory slot tooltip base class
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGInventory_Tooltip_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	URPGInventory_Tooltip_Widget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ItemThumbnailImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USizeBox* SizeBoxMainTooltip;

	UPROPERTY(BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(BlueprintReadOnly)
	FText ItemDescription;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* ItemThumbnailToSet;

};
