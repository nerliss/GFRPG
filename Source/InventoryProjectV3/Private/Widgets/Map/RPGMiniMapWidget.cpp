// Oleksandr Tkachov 2022-2023


#include "Widgets/Map/RPGMiniMapWidget.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Widgets/Map/RPGMapPlayerIconWidget.h"
#include "Utility/LogDefinitions.h"

void URPGMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MinimapMaterialParameterCollection, "Dimensions", Dimensions);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MinimapMaterialParameterCollection, "Zoom", ZoomLevel);

	UpdateMapImage(MapTexture);
	AddPlayerIcon();
}

void URPGMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateScalarParameters();
}

void URPGMiniMapWidget::AddPOI(AActor* POIActor)
{
	/*auto* POIComponent = POIActor->FindComponentByClass<URPGPointOfInterestComponent>();
	if (!POIComponent)
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMiniMapWidget::AddPOI] POI component is nullptr!"));
		return;
	}*/

	/*auto* POIIcon = CreateWidget<URPGMapPOIIconWidget>(GetWorld(), POIComponent->IconWidgetClass);
	if (!POIIcon)
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMiniMapWidget::AddPOI] POI Icon was not created, please specify IconWidgetClass!"));
		return;
	}

	auto* OverlaySlot = MapOverlay->AddChildToOverlay(POIIcon);
	if (!OverlaySlot)
	{
		return;
	}

	OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);*/
}

void URPGMiniMapWidget::UpdateMapImage(UTexture* NewMapTexture)
{
	MinimapMaterialInstance = Map->GetDynamicMaterial();
	MinimapMaterialInstance->SetTextureParameterValue("MapTexture", NewMapTexture);
}

void URPGMiniMapWidget::AddPlayerIcon()
{
	PlayerIcon = CreateWidget<URPGMapPlayerIconWidget>(GetWorld(), PlayerIconWidgetClass);
	if (!PlayerIcon)
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMiniMapWidget::AddPlayerIcon] Player Icon was not created, please specify PlayerIconWidgetClass!"));
		return;
	}

	auto* OverlaySlot = MapOverlay->AddChildToOverlay(PlayerIcon);
	if (!OverlaySlot)
	{
		return;
	}

	OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
}

void URPGMiniMapWidget::UpdateScalarParameters()
{
	auto* PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn)
	{
		return;
	}

	// Update map texture to show player icon on it
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MinimapMaterialParameterCollection, "X", PlayerPawn->GetActorLocation().X);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MinimapMaterialParameterCollection, "Y", PlayerPawn->GetActorLocation().Y);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MinimapMaterialParameterCollection, "Zoom", ZoomLevel);
}
