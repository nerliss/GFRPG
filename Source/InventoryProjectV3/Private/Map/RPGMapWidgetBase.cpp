// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapWidgetBase.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Map/RPGMapIconComponent.h"
#include "Map/RPGMapSubsystem.h"
#include "Utility/LogDefinitions.h"

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
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

}

int32 URPGMapWidgetBase::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	for (const FVector2D QuestMarker : QuestMarkers)
	{
		UWidgetBlueprintLibrary::DrawBox(Context, QuestMarker, FVector2D(32.f, 32.f), SlateBrushWaypoint, FLinearColor(1.f, 1.f, 1.f, 0.2f));
	}

	// Add here new waypoints if needed

	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
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
