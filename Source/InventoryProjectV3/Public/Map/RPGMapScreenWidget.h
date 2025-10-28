// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Map/RPGMapWidgetBase.h"
#include "RPGMapScreenWidget.generated.h"

class UScaleBox;

/**
 * Big map widget class (opens up upon pressing a button).
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGMapScreenWidget : public URPGMapWidgetBase
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown( const FGeometry& InGeometry, const FKeyEvent& InKeyEvent ) override;

	void UpdateZoom(const FPointerEvent& InMouseEvent);
	void UpdatePanning(const FPointerEvent& InMouseEvent);

	/* How fast should mouse wheel change zoom? */
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	float ZoomSpeed = 0.5f;

	/* How far can we zoom the map out? */
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	float ZoomMin = 0.5f;

	/* How close can we zoom the map in? */
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	float ZoomMax = 3.0f;

	/* Base pan speed which than scales with zoom */
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	float PanSpeed = 1.f;

	/* Pan borders for X axis */
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	FVector2D PanXMinMax = FVector2D(-900.f, 900.f);

	/* Pan borders for Y axis */
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	FVector2D PanYMinMax = FVector2D(-300.f, 300.f);
	
	UPROPERTY(meta=(BindWidgetOptional))
	UScaleBox* MapZoomBox;

	UPROPERTY(meta=(BindWidgetOptional))
	URPGMapWidgetBase* MapWidget;

private:

	float ZoomFactor;
	
};
