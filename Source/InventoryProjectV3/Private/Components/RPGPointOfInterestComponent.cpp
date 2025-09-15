// Oleksandr Tkachov 2022-2025


#include "Components/RPGPointOfInterestComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/RPGPlayer_Controller.h"
#include "Widgets/RPGHUD_Widget.h"
#include "Widgets/Map/RPGMiniMapWidget.h"
#include "Utility/LogDefinitions.h"

URPGPointOfInterestComponent::URPGPointOfInterestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Mobility = EPOIMobility::None;
	bQuestObjective = false;
}

void URPGPointOfInterestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bSpawned)
	{
		// Using new map system, don't spawn
		//SpawnPOIOnMap();
	}
}

void URPGPointOfInterestComponent::SpawnPOIOnMap()
{
	const auto* PlayerController = Cast<ARPGPlayer_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
	{
		// TODO: Remove this after everything is done
		UE_LOG(LogRPGMap, Error, TEXT("[URPGPointOfInterestComponent::SpawnPOIOnMap] Player controller is null!"));
		return;
	}

	const URPGHUD_Widget* HUD = PlayerController->GetHUDWidget();
	if (!HUD)
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGPointOfInterestComponent::SpawnPOIOnMap] HUD widget is null!"));
		return;
	}

	// URPGMiniMapWidget* Minimap = HUD->MiniMapWidget;
	// if (!Minimap)
	// {
	// 	UE_LOG(LogRPGMap, Error, TEXT("[URPGPointOfInterestComponent::SpawnPOIOnMap] Minimap is null!"));
	// 	return;
	// }
	//
	// Minimap->AddPOI(GetOwner());

	bSpawned = true;
	UE_LOG(LogRPGMap, Log, TEXT("[URPGPointOfInterestComponent::SpawnPOIOnMap] PointOfInterest icon for %s spawned"), *GetOwner()->GetName());
}

