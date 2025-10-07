// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapScreenWidget.h"

#include "Components/ScaleBox.h"

FReply URPGMapScreenWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bRightButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton);
	bLeftButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply URPGMapScreenWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bRightButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton);
	bLeftButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
	
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply URPGMapScreenWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UpdateZoom(InMouseEvent);
	
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

FReply URPGMapScreenWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void URPGMapScreenWidget::UpdateZoom(const FPointerEvent& InMouseEvent)
{
	if (!MapZoomBox)
	{
		return;
	}

	const float FinalZoomSpeed = InMouseEvent.GetWheelDelta() * ZoomSpeed;
	const float NewZoomBoxScale = MapZoomBox->GetUserSpecifiedScale() + FinalZoomSpeed;
	MapZoomBox->SetUserSpecifiedScale(FMath::Clamp(NewZoomBoxScale, ZoomMin, ZoomMax));
}

void URPGMapScreenWidget::UpdatePanning(const FPointerEvent& InMouseEvent)
{
	if (!MapWidget)
	{
		return;
	}

	if (!bRightButtonDown)
	{
		return;
	}

	const FVector2D PanLocation = MapWidget->GetRenderTransform().Translation + (InMouseEvent.GetCursorDelta() * PanSpeed);
	const FVector2D FinalPanLocation = FVector2D(FMath::Clamp(PanLocation.X, PanXMinMax.X, PanXMinMax.Y), FMath::Clamp(PanLocation.Y, PanYMinMax.X, PanYMinMax.Y));
	MapWidget->SetRenderTranslation(FinalPanLocation);
}
