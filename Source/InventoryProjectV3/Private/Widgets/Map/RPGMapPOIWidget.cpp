// Oleksandr Tkachov 2022-2025


#include "Widgets/Map/RPGMapPOIWidget.h"

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
#include "Widgets/RPGHUD_Widget.h"

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

void URPGMapPOIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// TODO: Remake for better since there is a lot of repetition from player poi pos logic. Move it to MapSubsystem or something. Also Map pointer here is really awkward
	if (Owner && POIImage)
	{
		ARPGPlayer_Controller* PlayerController = Cast<ARPGPlayer_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerController)
		{
			// Update icon visibility
			ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(PlayerController->GetCharacter());
			if (PlayerCharacter)
			{
				const bool bBelongsToMinimap = OwningMapWidget->bMiniMap;
				const float DistanceToPlayer = FVector::Distance(PlayerCharacter->GetActorLocation(), Owner->GetActorLocation());
				if (DistanceToPlayer >= 5000.f && bBelongsToMinimap)
				{
					if (POIImage->GetVisibility() != ESlateVisibility::Collapsed)
					{
						LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("Setting visibility to collapsed"));
						POIImage->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
				else
				{
					if (POIImage->GetVisibility() != ESlateVisibility::Visible)
					{
						LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("Setting visibility to visible"));
						POIImage->SetVisibility(ESlateVisibility::Visible);
					}
				}
			}
			
			URPGMiniMapWidget* Minimap = PlayerController->GetHUDWidget()->MiniMapWidget;
			if (Minimap)
			{
				const FVector OwnerLoc = Owner->GetActorLocation();
				const float OwnerIconX = OwnerLoc.X / Minimap->MapXDiv + Minimap->MapXOffset;
				const float OwnerIconY = OwnerLoc.Y / Minimap->MapYDiv + Minimap->MapYOffset;
				POIImage->SetRenderTranslation(FVector2D(OwnerIconX, OwnerIconY));

				URPGPointOfInterestComponent* OwnerPOIComp = Owner->FindComponentByClass<URPGPointOfInterestComponent>();
				if (OwnerPOIComp && OwnerPOIComp->bRotateWithActor)
				{
					const float OwnerIconAngle = Owner->GetActorRotation().Yaw + 90.f;
					POIImage->SetRenderTransformAngle(OwnerIconAngle); 
				}
			}
		}
	}
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
