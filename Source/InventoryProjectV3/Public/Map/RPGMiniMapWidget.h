// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "RPGMapWidgetBase.h"
#include "RPGMiniMapWidget.generated.h"

class URPGMapPOIWidget;
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

	virtual void InitMap() override;
	
	UFUNCTION()
	void AddWorldMarker(bool bSpawn, FVector2D MarkerLocation);
	
	virtual void AddPOI(AActor* Actor) override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGMapWidgetBase* MinimapWidget = nullptr;
};
