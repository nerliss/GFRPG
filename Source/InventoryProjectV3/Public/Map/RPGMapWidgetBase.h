// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGMapWidgetBase.generated.h"

class UImage;
class UCanvasPanel;
class URPGMapSubsystem;
class UDataTable;
class USlateBrushAsset;

UCLASS()
class INVENTORYPROJECTV3_API URPGMapWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;

	void InitMap();
	
	void UpdateQuestMarkers(FVector WaypointLocation);

	void VectorToPoint(FVector WaypointLocation, float& XValue, float& YValue);

	void UpdatePlayerPosition();

	void AddWaypoint(FVector WaypointLocation);
	
	UPROPERTY(meta=(BindWidgetOptional))
	UImage* MapImage = nullptr;

	/** Fog of War image */
	UPROPERTY(meta=(BindWidgetOptional))
	UImage* FogImage = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UImage* PlayerIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UDataTable* MapDataTable = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	USlateBrushAsset* SlateBrushWaypoint = nullptr;
	
	/** If true, rotate the whole map by player yaw (minimap style) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	bool bRotateWithPlayer = false;

	/** Scale of the map image. 1.0 fits to its brush size; >1 zooms in */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	float Zoom = 1.0f;

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

	// TODO: NYI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Map")
	TArray<FVector2D> QuestMarkers;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Map")
	bool bInitComplete = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Map")
	bool bLeftButtonDown = false;

	/* Set in a minimap widget that hosts this one */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	bool bIsMiniMap = false;
	
protected:
	
	TWeakObjectPtr<URPGMapSubsystem> MapSubsystem;
};
