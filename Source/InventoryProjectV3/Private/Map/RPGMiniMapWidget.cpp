// Oleksandr Tkachov 2022-2025


#include "Map/RPGMiniMapWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/RPGPointOfInterestComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Map/RPGMapSubsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/Map/RPGMapPlayerIconWidget.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"
#include "Widgets/Map/RPGMapPOIWidget.h"

void URPGMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	URPGMapSubsystem* MapSubsystem = GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	if (MapSubsystem)
	{
		MapSubsystem->OnWorldMarkerToggled.AddUObject(this, &URPGMiniMapWidget::AddWorldMarker);
	}
}

void URPGMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateMiniMapTranslation();
}

void URPGMiniMapWidget::UpdateMiniMapTranslation()
{
	const ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		if (MinimapWidget)
		{
			const float MinimapTranslationX = (PlayerCharacter->GetActorLocation().X + MinimapWidget->MapXOffset) / MinimapWidget->MapYDiv * -1.f;
			const float MinimapTranslationY = (PlayerCharacter->GetActorLocation().Y + MinimapWidget->MapYOffset) / MinimapWidget->MapXDiv * -1.f;
			
			MinimapWidget->SetRenderTranslation(FVector2D(MinimapTranslationX, MinimapTranslationY));
		}
	}
}

void URPGMiniMapWidget::InitMap()
{
	Super::InitMap();
	
	if (!MapDataTable)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("MapDataTable is empty"));
		return;
	}
	
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	const FMapValuesTableRow* FoundRow = MapDataTable->FindRow<FMapValuesTableRow>(*LevelName, TEXT("Map Table Context"));
	if (FoundRow)
	{
		if (FoundRow->bUseSeparateTextureForMinimap)
		{
			if (MinimapWidget && MinimapWidget->MapImage)
			{
				LOG_WITH_FUNCTION_NAME(LogRPGMap, VeryVerbose, TEXT("Settings separate texture for minimap"));
				MinimapWidget->MapImage->SetBrushFromTexture(FoundRow->MinimapTexture);
			}
		}
	}
}

void URPGMiniMapWidget::AddWorldMarker(bool bSpawn, FVector2D MarkerLocation)
{
	LOG_WITH_FUNCTION_NAME(LogRPGMap, Verbose, TEXT("Function called with params bSpawn = %s, MarkerLocation = %s"), *LexToString(bSpawn), *MarkerLocation.ToString());
	
	if (!MinimapWidget || !MinimapWidget->WorldMarker)
	{
		return;
	}
	
	if (bSpawn)
	{
		MinimapWidget->WorldMarker->SetVisibility(ESlateVisibility::Visible);
		MinimapWidget->WorldMarker->SetRenderTranslation(MarkerLocation);
	}
	else
	{
		MinimapWidget->WorldMarker->SetVisibility(ESlateVisibility::Hidden);
		MinimapWidget->WorldMarker->SetRenderTranslation(FVector2D(FLT_MAX));
	}
}

void URPGMiniMapWidget::AddPOI(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}

	if (!POIClass)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("POIClass is null"));
		return;
	}
	
	auto POIWidget = Cast<URPGMapPOIWidget>(CreateWidget(GetWorld(), POIClass));
	if (POIWidget)
	{
		auto POIComp = Actor->GetComponentByClass<URPGPointOfInterestComponent>();
		if (POIComp)
		{
			POIWidget->Owner = Actor;
			
			auto OverlaySlot = MinimapWidget->MapOverlay->AddChildToOverlay(POIWidget);
			if (OverlaySlot)
			{
				OverlaySlot->SetHorizontalAlignment(HAlign_Center);
				OverlaySlot->SetVerticalAlignment(VAlign_Center);
			}
			
			LOG_WITH_FUNCTION_NAME(LogRPGMap, Log, TEXT("Added POI for %s"), *Actor->GetName());
		}
	}
}
