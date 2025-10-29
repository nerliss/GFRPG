// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGMapWidgetBase.generated.h"

class URPGMapPOIWidget;
class UOverlay;
class UImage;
class UCanvasPanel;
class URPGMapSubsystem;
class UDataTable;
class USlateBrushAsset;

/**
 * Map widget base class which hosts all map related logic which then shared with MapScreen widget and MiniMap widget
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGMapWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;
	
	void AddWaypoint(FVector WaypointLocation);

	virtual void AddPOI(AActor* Actor, URPGMapWidgetBase* MapReference);
		
	UPROPERTY(meta=(BindWidgetOptional))
	UImage* MapImage = nullptr;

	/** Fog of War image */
	UPROPERTY(meta=(BindWidgetOptional))
	UImage* FogImage = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UOverlay* MapOverlay = nullptr;

	// TODO: Remove
	UPROPERTY(meta=(BindWidgetOptional))
	UImage* PlayerIcon = nullptr;

	// TODO: Remake to use Point of Interest component
	UPROPERTY(meta=(BindWidgetOptional))
	UImage* WorldMarker = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UDataTable* MapDataTable = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	USlateBrushAsset* SlateBrushWaypoint = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Map")
	float MapXDiv = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Map")
	float MapYDiv = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Map")
	float MapXOffset = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Map")
	float MapYOffset = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Map")
	float WidgetMapSize = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Map")
	float WidgetHalfSize = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Map")
	float WorldIconHalfSize = 16.f;

	// TODO: NYI - Probably won't need
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Map")
	TArray<FVector2D> QuestMarkers;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Map")
	bool bInitComplete = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Map")
	bool bLeftButtonDown = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Map")
	bool bRightButtonDown = false;
	
	/* Set in a minimap widget that hosts this one */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	bool bMiniMap = false;

	UPROPERTY()
	TArray<URPGMapPOIWidget*> MapPOIWidgets;

	UPROPERTY()
	URPGMapPOIWidget* PlayerPOI = nullptr;
	
protected:

	virtual void InitMap();
	
private:

	void VectorToPoint(FVector WaypointLocation, float& XValue, float& YValue);
	void UpdateQuestMarkers(FVector WaypointLocation);
	void UpdatePlayerPosition();
	void ToggleWorldMarker(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	
};
