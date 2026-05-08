// Oleksandr Tkachov 2021-2026


#include "Map/RPGMapPOIWidget.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGPointOfInterestComponent.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Map/RPGMapSubsystem.h"
#include "Map/RPGMiniMapWidget.h"
#include "PlayerController/RPGPlayer_Controller.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

void URPGMapPOIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetPOIIcon();
	UpdateTooltipText();

	URPGMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	if (MapSubsystem)
	{
		MapSubsystem->OnMapZoomChanged.AddUObject(this, &URPGMapPOIWidget::UpdateIconSize);
	}
}

void URPGMapPOIWidget::NativeDestruct()
{
	LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("Destroying"));
	Super::NativeDestruct();
}

void URPGMapPOIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateIconVisibility();
	UpdateIconTransform();
}

void URPGMapPOIWidget::UpdateIconSize(float ZoomFactor)
{
	if (OwningMapWidget && OwningMapWidget->bMiniMap)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, VeryVerbose, TEXT("Ignoring icon change by zoom on Minimap"));
		return;
	}
	
	LOG_WITH_FUNCTION_NAME(LogRPGMap, VeryVerbose, TEXT("ZoomFactor = %f"), ZoomFactor);

	if (POIImage)
	{
		POIImage->SetRenderScale(FVector2D(1.f / ZoomFactor));
	}
}

void URPGMapPOIWidget::UpdateIconVisibility()
{
	if (!Owner)
	{
		return;
	}
	
	if (!POIImage)
	{
		return;
	}
	
	const ARPGPlayer_Controller* PlayerController = Cast<ARPGPlayer_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		return;
	}

	const URPGPointOfInterestComponent* OwnerPOIComp = Owner->FindComponentByClass<URPGPointOfInterestComponent>();
	if (!OwnerPOIComp)
	{
		return;
	}

	if (OwnerPOIComp->bAlwaysShow)
	{
		if (POIImage->GetVisibility() != ESlateVisibility::Visible)
		{
			LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("Setting visibility to visible since the Actor (%s) must always be visible"), *Owner->GetName());
			POIImage->SetVisibility(ESlateVisibility::Visible);
		}
		return;
	}

	ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(PlayerController->GetCharacter());
	if (!PlayerCharacter)
	{
		return;
	}
	
	const bool bBelongsToMinimap = OwningMapWidget->bMiniMap;
	const float DistanceToPlayer = FVector::Distance(PlayerCharacter->GetActorLocation(), Owner->GetActorLocation());
	if (DistanceToPlayer >= 5000.f && bBelongsToMinimap)
	{
		if (POIImage->GetVisibility() != ESlateVisibility::Collapsed)
		{
			LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("Setting visibility to collapsed"));
			POIImage->SetVisibility(ESlateVisibility::Collapsed);
		}
		return;
	}

	if (POIImage->GetVisibility() != ESlateVisibility::Visible)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("Setting visibility to visible"));
		POIImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void URPGMapPOIWidget::UpdateIconTransform()
{
	if (!Owner)
	{
		return;
	}

	if (!POIImage)
	{
		return;
	}
	
	URPGMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	if (!MapSubsystem)
	{
		return;
	}

	const URPGPointOfInterestComponent* OwnerPOIComp = Owner->FindComponentByClass<URPGPointOfInterestComponent>();
	if (!OwnerPOIComp)
	{
		return;
	}
	
	const FVector OwnerLoc = Owner->GetActorLocation();
	const float OwnerIconX = OwnerLoc.X / MapSubsystem->MapDimensions.MapXDiv + MapSubsystem->MapDimensions.MapXOffset;
	const float OwnerIconY = OwnerLoc.Y / MapSubsystem->MapDimensions.MapYDiv + MapSubsystem->MapDimensions.MapYOffset;
	POIImage->SetRenderTranslation(FVector2D(OwnerIconX, OwnerIconY));

	if (OwnerPOIComp->bRotateWithActor)
	{
		const float OwnerIconAngle = Owner->GetActorRotation().Yaw + 90.f;
		POIImage->SetRenderTransformAngle(OwnerIconAngle);
	}
}

void URPGMapPOIWidget::SetPOIIcon()
{
	if (!Owner)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Owner is nullptr"));
		return;
	}

	const URPGPointOfInterestComponent* OwnerPOIComponent = Owner->FindComponentByClass<URPGPointOfInterestComponent>();
	if (!OwnerPOIComponent)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Owner doesn't have a URPGPointOfInterestComponent component"));
		return;
	}

	POIImage->SetBrush(OwnerPOIComponent->GetIcon());
	bIconSet = true;
	LOG_WITH_FUNCTION_NAME(LogRPGMap, Log, TEXT("Setting POI Image as (%s) for component (%s) of owning actor (%s)"), *OwnerPOIComponent->GetIcon().GetResourceName().ToString(), *GetName(), *Owner->GetName());
}

void URPGMapPOIWidget::UpdateTooltipText()
{
	const IRPGInteract_Interface* InteractActorCasted = Cast<IRPGInteract_Interface>(Owner);
	if (!InteractActorCasted)
	{
		return;
	}
	
	SetToolTipText(InteractActorCasted->GetNameNative());
}
