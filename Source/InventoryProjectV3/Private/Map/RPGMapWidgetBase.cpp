// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapWidgetBase.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Map/RPGMapSubsystem.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

void URPGMapWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	InitMap();
}

void URPGMapWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdatePlayerPosition();
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

int32 URPGMapWidgetBase::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                     const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                     const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	for (const FVector2D QuestMarker : QuestMarkers)
	{
		UWidgetBlueprintLibrary::DrawBox(Context, QuestMarker, FVector2D(32.f, 32.f), SlateBrushWaypoint, FLinearColor(1.f, 1.f, 1.f, 0.2f));
	}

	// Add here new waypoints if needed

	return LayerId;
}

void URPGMapWidgetBase::InitMap()
{
	if (bInitComplete)
	{
		UE_LOG(LogRPGMap, Warning, TEXT("[URPGMapWidgetBase::InitMap] Map is already initialized!"));
		return;
	}

	if (!MapDataTable)
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMapWidgetBase::InitMap] MapDataTable is empty"));
		return;
	}
	
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	const FMapValuesTableRow* FoundRow = MapDataTable->FindRow<FMapValuesTableRow>(*LevelName, TEXT("Map Table Context"));
	if (FoundRow)
	{
		MapXDiv = FoundRow->MapSizeX / WidgetMapSize;
		MapYDiv = FoundRow->MapSizeY / WidgetMapSize;
		MapXOffset = FoundRow->MapOffsetX / MapXDiv;
		MapYOffset = FoundRow->MapOffsetY / MapYDiv;

		if (MapImage)
		{
			MapImage->SetBrushFromTexture(FoundRow->MapTexture);
		}
	}

	bInitComplete = true;
}

void URPGMapWidgetBase::UpdateQuestMarkers(FVector WaypointLocation)
{
	float X, Y;
	VectorToPoint(WaypointLocation, X, Y);
	
	QuestMarkers.Add(FVector2D(X, Y));
}

void URPGMapWidgetBase::VectorToPoint(FVector WaypointLocation, float& XValue, float& YValue)
{
	// Converts the provided waypoint location to the correct X and Y based on the map UI size
	XValue = ((((WaypointLocation.X / MapXDiv) + MapXOffset) - WorldIconHalfSize) + 500.f);
	YValue = ((((WaypointLocation.Y / MapYDiv) + MapYOffset) - WorldIconHalfSize) + 500.f);
}

void URPGMapWidgetBase::UpdatePlayerPosition()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter && PlayerCharacter->GetMesh() && PlayerIcon)
	{
		const FVector PlayerLoc = PlayerCharacter->GetActorLocation();
		const float PlayerIconX = PlayerLoc.X / MapXDiv + MapXOffset;
		const float PlayerIconY = PlayerLoc.Y / MapYDiv + MapYOffset;

		const float PlayerIconAngle = PlayerCharacter->GetMesh()->GetComponentRotation().Yaw + 90.f;
		
		PlayerIcon->SetRenderTranslation(FVector2D(PlayerIconX, PlayerIconY));
		PlayerIcon->SetRenderTransformAngle(PlayerIconAngle);
	}
}

void URPGMapWidgetBase::AddWaypoint(FVector WaypointLocation)
{
	if (bInitComplete)
	{
		UpdateQuestMarkers(WaypointLocation);
	}
}

void URPGMapWidgetBase::ToggleWorldMarker(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bLeftButtonDown)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Left mouse button is not pressed"));
		return;
	}

	if (!WorldMarker)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("WorldMarker is nullptr"));
		return;
	}
	
	if (bMiniMap)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Called on minimap, do nothing"));
		return;
	}
	
	const FVector2D CursorLocalCoords = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	const float WorldMarkerX = WorldMarker->GetRenderTransform().Translation.X;
	const float WorldMarkerY = WorldMarker->GetRenderTransform().Translation.Y;
	const float CursorPositionOnWidgetX = CursorLocalCoords.X - WidgetHalfSize;
	const float CursorPositionOnWidgetY = CursorLocalCoords.Y - WidgetHalfSize;
	const float CursorInMarkerRangeForXMin = CursorPositionOnWidgetX - 10.f;
	const float CursorInMarkerRangeForXMax = CursorPositionOnWidgetX + 10.f;
	const float CursorInMarkerRangeForYMin = CursorPositionOnWidgetY - 10.f;
	const float CursorInMarkerRangeForYMax = CursorPositionOnWidgetY + 10.f;
	const bool bCursorInMarkerRangeForX = ((CursorInMarkerRangeForXMin <= WorldMarkerX) && (CursorInMarkerRangeForXMax >= WorldMarkerX));
	const bool bCursorInMarkerRangeForY = ((CursorInMarkerRangeForYMin <= WorldMarkerY) && (CursorInMarkerRangeForYMax >= WorldMarkerY));
	const bool bRemoveWorldMaker = bCursorInMarkerRangeForX && bCursorInMarkerRangeForY;

	LOG_WITH_FUNCTION_NAME(LogRPGMap, Verbose, TEXT("Toggling world marker at X=%f, Y=%f (Cursor's position: x = %f, y = %f. Removing marker = %s."), CursorPositionOnWidgetX, CursorPositionOnWidgetY, CursorLocalCoords.X, CursorLocalCoords.Y, *LexToString(bRemoveWorldMaker));
	
	if (bRemoveWorldMaker)
	{
		WorldMarker->SetVisibility(ESlateVisibility::Hidden);
		WorldMarker->SetRenderTranslation(FVector2D(FLT_MAX));

		URPGMapSubsystem* MapSubsystem = GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
		if (MapSubsystem)
		{
			MapSubsystem->OnWorldMarkerToggled.Broadcast(false, FVector2D(0.f));
		}
		
		// TODO: Spawn world marker interface call with 0.f coords
	}
	else
	{
		WorldMarker->SetVisibility(ESlateVisibility::Visible);
		WorldMarker->SetRenderTranslation(FVector2D(CursorPositionOnWidgetX, CursorPositionOnWidgetY));

		URPGMapSubsystem* MapSubsystem = GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
		if (MapSubsystem)
		{
			MapSubsystem->OnWorldMarkerToggled.Broadcast(true, FVector2D(CursorPositionOnWidgetX, CursorPositionOnWidgetY));
		}
		
		FVector NewWorldMarkerLocation = FVector((CursorPositionOnWidgetX - MapXOffset) * MapXDiv, (CursorPositionOnWidgetY - MapYOffset) * MapYDiv, 0.f);
		// TODO: Spawn world marker interface call with NewWorldMarkerLocation
	}
}
