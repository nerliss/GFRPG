// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGInventoryTooltipWidget.generated.h"

class UTexture2D;
class UImage;
class USizeBox;

/**
 * Inventory slot tooltip base class
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGInventoryTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	URPGInventoryTooltipWidget(const FObjectInitializer& ObjectInitializer);

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
