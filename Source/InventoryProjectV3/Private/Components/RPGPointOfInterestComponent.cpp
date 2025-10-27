// Oleksandr Tkachov 2022-2025


#include "Components/RPGPointOfInterestComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Map/RPGMapScreenWidget.h"
#include "Map/RPGMapSubsystem.h"
#include "PlayerController/RPGPlayer_Controller.h"
#include "Widgets/RPGHUD_Widget.h"
#include "Map/RPGMiniMapWidget.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

URPGPointOfInterestComponent::URPGPointOfInterestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Icon.SetImageSize(FVector2D(16.0f, 16.0f));
	Mobility = EPOIMobility::None;
	bQuestObjective = false;
}

void URPGPointOfInterestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bSpawned)
	{
		SpawnPOIOnMap();
	}
}

void URPGPointOfInterestComponent::SpawnPOIOnMap()
{
	const ARPGPlayer_Controller* PlayerController = Cast<ARPGPlayer_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Player controller is null!"));
		return;
	}

	const URPGHUD_Widget* HUD = PlayerController->GetHUDWidget();
	if (!HUD)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("HUD widget is null!"));
		return;
	}

	URPGMiniMapWidget* Minimap = HUD->MiniMapWidget;
	if (!Minimap)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Minimap is null!"));
		return;
	}

	// TODO: Try to unify it more, may be move minimap logic to map subsystem too
	Minimap->AddPOI(GetOwner(), Minimap);

	auto MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	if (MapSubsystem && MapSubsystem->MapScreenWidget)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Log, TEXT("Spawning on MapScreen too"))
		MapSubsystem->MapScreenWidget->AddPOI(GetOwner(), MapSubsystem->MapScreenWidget);
	}
	else
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Map subsystem or its MapScreenWidget is null!"));
	}

	bSpawned = true;
	LOG_WITH_FUNCTION_NAME(LogRPGMap, Log, TEXT("PointOfInterest icon for %s spawned"), *GetOwner()->GetName());
}

