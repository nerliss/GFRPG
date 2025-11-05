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
	bRotateWithActor = false;
	bAlwaysShow = false;
	
	bSpawned = false;
}

void URPGPointOfInterestComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Don't want to remove if the object was streamed out
	if (EndPlayReason != EEndPlayReason::Type::RemovedFromWorld)
	{
		auto MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
		if (MapSubsystem)
		{
			MapSubsystem->MapScreenWidget->CleanupPOIWidgets();
			//MapSubsystem->OnPointOfInterestComponentDestroyed.Broadcast(Cast<AActor>(GetOuter()));
		}
	}

	Super::EndPlay(EndPlayReason);
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
	if (!GetIcon().GetResourceObject())
	{
		bSpawned = true;
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("Actor %s doesn't have an icon to spawn"), *GetOuter()->GetName());
		return;
	}
	
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

	URPGMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	if (MapSubsystem && MapSubsystem->MapScreenWidget)
	{
		MapSubsystem->MapScreenWidget->AddPOI(GetOwner(), MapSubsystem->MapScreenWidget);
	}

	bSpawned = true;
	LOG_WITH_FUNCTION_NAME(LogRPGMap, Log, TEXT("PointOfInterest icon for %s spawned"), *GetOwner()->GetName());
}

