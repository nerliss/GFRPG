// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapWidgetBase.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/RPGPointOfInterestComponent.h"
#include "Map/RPG3DWorldMarker.h"
#include "Map/RPGMapSubsystem.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"
#include "Map/RPGMapPOIWidget.h"

void URPGMapWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	InitMap();
}

void URPGMapWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CleanupPOIWidgets();
}

FReply URPGMapWidgetBase::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	LOG_WITH_FUNCTION_NAME(LogRPGMap, VeryVerbose, TEXT("Parent Called"));
	ToggleWorldMarker(InGeometry, InMouseEvent);

	// TODO: Hard reset mb?
	bLeftButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
	bRightButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton);
	
	return FReply::Handled(); 
}

FReply URPGMapWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	bLeftButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
	bRightButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton);
	
	LOG_WITH_FUNCTION_NAME(LogRPGMap, VeryVerbose, TEXT("Parent Called"));
	
	return FReply::Handled();
}

void URPGMapWidgetBase::InitMap()
{
	if (bInitComplete)
	{
		UE_LOG(LogRPGMap, Warning, TEXT("[URPGMapWidgetBase::InitMap] Map is already initialized!"));
		return;
	}
	
	URPGMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	if (!MapSubsystem)
	{
		return;
	}

	const UMapSubsystemSettings* MapSettings = UMapSubsystemSettings::Get();
	check(MapSettings);

	WidgetMapSize = MapSettings->WidgetMapSize;
	WidgetHalfSize = WidgetMapSize / 2;
	
	MapDimensions = MapSubsystem->GetMapDimensions();
	
	const FMapValuesTableRow* FoundRow = MapSubsystem->GetMapValuesTableRow();
	if (FoundRow && MapImage)
	{
		MapImage->SetBrushFromTexture(FoundRow->MapTexture);
	}
	
	bInitComplete = true;
}

void URPGMapWidgetBase::UpdateQuestMarkers(FVector WaypointLocation)
{
	float X, Y;
	VectorToPoint(WaypointLocation, X, Y);
}

void URPGMapWidgetBase::VectorToPoint(FVector WaypointLocation, float& XValue, float& YValue)
{
	// Converts the provided waypoint location to the correct X and Y based on the map UI size
	XValue = ((((WaypointLocation.X / MapDimensions.MapXDiv) + MapDimensions.MapXOffset) - WorldIconHalfSize) + 500.f);
	YValue = ((((WaypointLocation.Y / MapDimensions.MapYDiv) + MapDimensions.MapYOffset) - WorldIconHalfSize) + 500.f);
}

void URPGMapWidgetBase::AddWaypoint(FVector WaypointLocation)
{
	if (bInitComplete)
	{
		UpdateQuestMarkers(WaypointLocation);
	}
}

void URPGMapWidgetBase::AddPOI(AActor* Actor, URPGMapWidgetBase* MapReference)
{
	if (!Actor)
	{
		return;
	}

	const UMapSubsystemSettings* MapSettings = UMapSubsystemSettings::Get();
	check(MapSettings);
	
	if (!MapSettings->POIWidgetClass)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("POIClass is null"));
		return;
	}

	URPGMapPOIWidget* POIWidget = Cast<URPGMapPOIWidget>(CreateWidget(GetWorld(), MapSettings->POIWidgetClass));
	if (POIWidget)
	{
		URPGPointOfInterestComponent* POIComp = Actor->GetComponentByClass<URPGPointOfInterestComponent>();
		if (POIComp)
		{
			POIWidget->Owner = Actor;
			POIWidget->OwningMapWidget = MapReference; // Also works with 'this', but better to leave it as explicitly as this

			//POIWidget->GetToolTipText()
			// TODO: Make it more versatile
			if (bMiniMap)
			{
				POIWidget->POIImage->SetRenderScale(FVector2D(1.f / 5)); // 5 is currently a user specified scale for ScaleBox for Minimap
			}
			
			MapPOIWidgets.AddUnique(POIWidget);

			URPGMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
			if (MapSubsystem)
			{
				// Broadcast only for MapScreen 
				if (!bMiniMap)
				{
					MapSubsystem->OnPointOfInterestIconSpawned.Broadcast(POIWidget);
				}
				
				MapSubsystem->OnPointOfInterestComponentDestroyed.AddUObject(this, &URPGMapWidgetBase::RemovePOI);
			}
			
			// Save player's icon separately
			ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(Actor);
			if (PlayerCharacter)
			{
				LOG_WITH_FUNCTION_NAME(LogRPGMap, VeryVerbose, TEXT("%s is player's icon"), *POIWidget->GetName());
				PlayerPOI = POIWidget;
			}
			
			if (MapOverlay)
			{
				UOverlaySlot* OverlaySlot = MapOverlay->AddChildToOverlay(POIWidget);
				if (OverlaySlot)
				{
					OverlaySlot->SetHorizontalAlignment(HAlign_Center);
					OverlaySlot->SetVerticalAlignment(VAlign_Center);
				}

				MovePlayerIconToTop();
			}
			
			LOG_WITH_FUNCTION_NAME(LogRPGMap, Log, TEXT("PARENT CALL: Added POI for %s"), *Actor->GetName());
		}
	}
}

void URPGMapWidgetBase::RemovePOI(AActor* Actor, URPGMapWidgetBase* MapReference)
{
	
}

void URPGMapWidgetBase::CleanupPOIWidgets()
{
	for (URPGMapPOIWidget* MapPOIWidget : MapPOIWidgets)
	{
		if (MapPOIWidget)
		{
			// Should be fine for now
			if (!IsValid(MapPOIWidget->Owner))
			{
				LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("Removing icon %s"), *MapPOIWidget->GetName());
				MapPOIWidgets.Remove(MapPOIWidget);
				MapPOIWidget->RemoveFromParent();
				return; // Trying to escape ensure condition
			}
		}
	}
}

void URPGMapWidgetBase::ToggleWorldMarker(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// TODO: Remake to not utilize UImage WorldMarker ptr
	if (!bLeftButtonDown)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Left mouse button is not pressed"));
		return;
	}
		
	if (bMiniMap)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Called on minimap, do nothing"));
		return;
	}

	URPGMapSubsystem* MapSubsystem = GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	if (!MapSubsystem)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("MapSubsystem is nullptr"));
		return;
	}
	
	const FVector2D CursorLocalCoords = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	//const float WorldMarkerX = WorldMarker->GetRenderTransform().Translation.X;
	//const float WorldMarkerY = WorldMarker->GetRenderTransform().Translation.Y;
	const float CursorPositionOnWidgetX = CursorLocalCoords.X - WidgetHalfSize;
	const float CursorPositionOnWidgetY = CursorLocalCoords.Y - WidgetHalfSize;
	const float CursorInMarkerRangeForXMin = CursorPositionOnWidgetX - 10.f;
	const float CursorInMarkerRangeForXMax = CursorPositionOnWidgetX + 10.f;
	const float CursorInMarkerRangeForYMin = CursorPositionOnWidgetY - 10.f;
	const float CursorInMarkerRangeForYMax = CursorPositionOnWidgetY + 10.f;
	// const bool bCursorInMarkerRangeForX = ((CursorInMarkerRangeForXMin <= WorldMarkerX) && (CursorInMarkerRangeForXMax >= WorldMarkerX));
	// const bool bCursorInMarkerRangeForY = ((CursorInMarkerRangeForYMin <= WorldMarkerY) && (CursorInMarkerRangeForYMax >= WorldMarkerY));
	// const bool bRemoveWorldMaker = bCursorInMarkerRangeForX && bCursorInMarkerRangeForY;
	
	//LOG_WITH_FUNCTION_NAME(LogRPGMap, Verbose, TEXT("Toggling world marker at X=%f, Y=%f (Cursor's position: x = %f, y = %f. Removing marker = %s."), CursorPositionOnWidgetX, CursorPositionOnWidgetY, CursorLocalCoords.X, CursorLocalCoords.Y, *LexToString(bRemoveWorldMaker));
	
	// if (bRemoveWorldMaker)
	// {
	// 	// WorldMarker->SetVisibility(ESlateVisibility::Hidden);
	// 	// WorldMarker->SetRenderTranslation(FVector2D(FLT_MAX));
	// 	
	// 	MapSubsystem->On3DWorldMarkerSpawned.Broadcast(false, FVector(0.f));
	// }
	// else
	// {
		// WorldMarker->SetVisibility(ESlateVisibility::Visible);
		// WorldMarker->SetRenderTranslation(FVector2D(CursorPositionOnWidgetX, CursorPositionOnWidgetY));
		//
	const FVector NewWorldMarkerLocation = FVector((CursorPositionOnWidgetX - MapDimensions.MapXOffset) * MapDimensions.MapXDiv, (CursorPositionOnWidgetY - MapDimensions.MapYOffset) * MapDimensions.MapYDiv, 0.f);
	const FVector WorldMarkerLocation = MapSubsystem->WorldMarker ? MapSubsystem->WorldMarker->GetActorLocation() : FVector(0.f);
	//MapSubsystem->On3DWorldMarkerSpawned.Broadcast(true, NewWorldMarkerLocation);
	 float DistanceBetweenMarkerLocations = FVector::Distance(WorldMarkerLocation, NewWorldMarkerLocation);
	 if (DistanceBetweenMarkerLocations < 250.f)
	 {
	 	MapSubsystem->On3DWorldMarkerSpawned.Broadcast(false, FVector(0.f));
	 }
	 else
	 {
	 	MapSubsystem->On3DWorldMarkerSpawned.Broadcast(true, NewWorldMarkerLocation);
	 }
	
	 LOG_WITH_FUNCTION_NAME(LogRPGMap, Display, TEXT("Distance between WorldMarker and its new desired position is %f"), DistanceBetweenMarkerLocations);	
	
	//}
}

void URPGMapWidgetBase::MovePlayerIconToTop()
{
	if (!PlayerPOI)
	{
		return;
	}
	
	// Ensure the player icon is always on top to have the highest render priority (since ZOrder is not available)
	if (MapOverlay->GetChildIndex(PlayerPOI) != INDEX_NONE)
	{
		MapOverlay->RemoveChildAt(MapOverlay->GetChildIndex(PlayerPOI));

		UOverlaySlot* PlayerIconSlot = MapOverlay->AddChildToOverlay(PlayerPOI);
		if (PlayerIconSlot)
		{
			PlayerIconSlot->SetHorizontalAlignment(HAlign_Center);
			PlayerIconSlot->SetVerticalAlignment(VAlign_Center);
		}
	}
}
