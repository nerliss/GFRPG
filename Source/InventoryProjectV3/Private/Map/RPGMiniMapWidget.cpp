// Oleksandr Tkachov 2022-2025


#include "Map/RPGMiniMapWidget.h"
#include "Components/Image.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Map/RPGMapSubsystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/Map/RPGMapPlayerIconWidget.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

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
