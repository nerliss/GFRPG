// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGMiniMapWidget.generated.h"

class UOverlay;
class UImage;
class UTexture;
class UMaterialParameterCollection;
class URPGMapPlayerIconWidget;

/**
 * Minimap widget base class
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* MapOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Map;

	UPROPERTY(EditDefaultsOnly)
	UMaterialParameterCollection* MinimapMaterialParameterCollection;

	// Using EditInstanceOnly since the map widget is created in RPGHUD_Widget blueprint and updating data here would not update it in that instance
	/* Current zoom level */
	UPROPERTY(EditInstanceOnly)
	float ZoomLevel;

	// MyTODO: Figure out how to update that automatically with the current world bounds
	/* Should be as big as the current world bounds */
	UPROPERTY(EditInstanceOnly)
	float Dimensions;

	UPROPERTY(EditInstanceOnly)
	UTexture* MapTexture = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URPGMapPlayerIconWidget> PlayerIconWidgetClass;

	URPGMapPlayerIconWidget* PlayerIcon;

	UMaterialInstanceDynamic* MinimapMaterialInstance;

	void UpdateMapImage(UTexture* NewMapTexture);
	void AddPlayerIcon();
	void UpdateScalarParameters();
};
