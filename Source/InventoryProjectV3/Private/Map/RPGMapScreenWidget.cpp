// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapScreenWidget.h"

#include "Components/ScaleBox.h"
#include "GameFramework/InputSettings.h"
#include "Map/RPGMapSubsystem.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

void URPGMapScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Assign map's world marker ptr to this class' world marker ptr since the latter is null hence world marker logic doesn't work
	WorldMarker = MapWidget->WorldMarker;
	MapOverlay = MapWidget->MapOverlay;
}

void URPGMapScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
}

FReply URPGMapScreenWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	
	UpdateZoom(InMouseEvent);
	
	return FReply::Handled(); 
}

FReply URPGMapScreenWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	
	UpdatePanning(InMouseEvent);
	
	return FReply::Handled(); 
}

void URPGMapScreenWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bRightButtonDown = false;
	bLeftButtonDown = false;
}

FReply URPGMapScreenWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	TArray<FInputActionKeyMapping> KeyMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName("ToggleMapScreen", KeyMappings);

	for (FInputActionKeyMapping& Key : KeyMappings)
	{
		if (Key.Key == InKeyEvent.GetKey())
		{
			URPGMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
			if (MapSubsystem)
			{
				MapSubsystem->ToggleMapScreen();
				break;
			}
		}
	}
	
	return FReply::Handled();
}

void URPGMapScreenWidget::UpdateZoom(const FPointerEvent& InMouseEvent)
{
	if (!MapZoomBox)
	{
		return;
	}

	const float ZoomStep = InMouseEvent.GetWheelDelta() * ZoomSpeed;
	const float NewZoomBoxScale = MapZoomBox->GetUserSpecifiedScale() + ZoomStep;
	ZoomFactor = FMath::Clamp(NewZoomBoxScale, ZoomMin, ZoomMax);

	if (ZoomFactor == PreviousZoomFactor)
	{
		return;
	}
	
	MapZoomBox->SetUserSpecifiedScale(ZoomFactor);

	URPGMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	if (MapSubsystem)
	{
		MapSubsystem->OnMapZoomChanged.Broadcast(ZoomFactor);
	}

	PreviousZoomFactor = ZoomFactor;
}

void URPGMapScreenWidget::UpdatePanning(const FPointerEvent& InMouseEvent)
{
	if (!MapWidget)
	{
		return;
	}

	bRightButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton);
	if (!bRightButtonDown)
	{
		return;
	}

	const float FinalPanSpeed = PanSpeed / ZoomFactor;
	const FVector2D PanLocation = MapWidget->GetRenderTransform().Translation + (InMouseEvent.GetCursorDelta() * FinalPanSpeed);
	const FVector2D FinalPanLocation = FVector2D(FMath::Clamp(PanLocation.X, PanXMinMax.X, PanXMinMax.Y), FMath::Clamp(PanLocation.Y, PanYMinMax.X, PanYMinMax.Y));
	MapWidget->SetRenderTranslation(FinalPanLocation);
}
