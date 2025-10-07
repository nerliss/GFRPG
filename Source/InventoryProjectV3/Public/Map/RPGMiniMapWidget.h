// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "RPGMapWidgetBase.h"
#include "RPGMiniMapWidget.generated.h"

/**
 * Minimap widget class
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGMiniMapWidget : public URPGMapWidgetBase
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateMiniMapTranslation();

	UFUNCTION()
	void AddWorldMarker(bool bSpawn, FVector2D MarkerLocation);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGMapWidgetBase* MinimapWidget;
	
};
