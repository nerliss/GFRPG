// Oleksandr Tkachov 2021-2026


#include "Map/RPGMapSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Map/RPG3DWorldMarker.h"
#include "Utility/LogDefinitions.h"
#include "Map/RPGMapScreenWidget.h"
#include "PlayerController/RPGPlayer_Controller.h"
#include "Utility/Utility.h"

void URPGMapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UpdateMapDimensions();
	InitializeMapScreen();
	
	On3DWorldMarkerSpawned.AddUObject(this, &URPGMapSubsystem::Spawn3DWorldMarker);
}

void URPGMapSubsystem::RevealAtWorld(const FVector& World, float RadiusWorldUnits, float Opacity)
{
	if (!FogRT)
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMapSubsystem::RevealAtWorld] FogRT is null - whether is not set up, or is turned off"));
		return;
	}

	// FVector2D UV;
	// if (!WorldToMapUV(World, UV))
	// {
	// 	UE_LOG(LogRPGMap, Error, TEXT("[URPGMapSubsystem::RevealAtWorld] WorldToMapUV returned false"));
	// 	return;
	// }

	// Convert world radius to UV radius using bounds max dimension
	FVector2D Min, Max;
	const FVector2D Size = Max - Min;
	const float RadiusUV = RadiusWorldUnits / FMath::Max(Size.X, Size.Y);
	
	// TODO: create a material for fog, finish function here for now
	
	// A small "paint" material that draws a soft white circle into FogRT
	// static TWeakObjectPtr PaintMatBase = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/UI/Map/M_MapFogPaint.M_MapFogPaint"));
	// if (PaintMatBase.IsValid())
	// {
	// 	// TODO: Add these parameters
	// 	UMaterialInstanceDynamic* DynamicMI = UMaterialInstanceDynamic::Create(PaintMatBase.Get(), this);
	// 	DynamicMI->SetVectorParameterValue(TEXT("UV"), FLinearColor(UV.X, UV.Y, 0.f, 0.f));
	// 	DynamicMI->SetScalarParameterValue(TEXT("RadiusUV"), RadiusUV);
	// 	DynamicMI->SetScalarParameterValue(TEXT("Opacity"), Opacity);
	//
	// 	UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), FogRT, DynamicMI);
	// }
	// else
	// {
	// 	PaintMatBase = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/UI/Map/M_MapFogPaint.M_MapFogPaint"));
	// 	UE_LOG (LogRPGMap, Error, TEXT("[URPGMapSubsystem::RevealAtWorld] PaintMatBase didn't load twice. Check if the material path is correct"))
	// }
}

void URPGMapSubsystem::Spawn3DWorldMarker(bool bSpawn, FVector Location)
{
	LOG_WITH_FUNCTION_NAME(LogRPGMap, Verbose, TEXT("Spawning 3d World Marker"));

	const UMapSubsystemSettings* MapSettings = UMapSubsystemSettings::Get();
	check(MapSettings);
	
	if (bSpawn)
	{
		if (WorldMarker)
		{
			WorldMarker->Destroy();
		}
	
		FHitResult Hit;
		FCollisionObjectQueryParams Params;
		Params.AddObjectTypesToQuery(ECC_WorldStatic);
		Params.AddObjectTypesToQuery(ECC_WorldDynamic);
		const FVector StartLocation = FVector(Location.X, Location.Y, Location.Z + 250000.f);
		const FVector EndLocation = FVector(Location.X, Location.Y, Location.Z - 500000.f);
		if (GetWorld()->LineTraceSingleByObjectType(Hit, StartLocation, EndLocation, Params))
		{
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Black, false, 10.f, 0, 20);
			
			WorldMarker = GetWorld()->SpawnActor<ARPG3DWorldMarker>(MapSettings->RPG3DWorldMarker, Hit.ImpactPoint, FRotator(0.f));
			if (WorldMarker)
			{
				WorldMarker->SetOwner(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
				WorldMarker->bQuestWaypoint = false;
				WorldMarker->WaypointName = FText::FromString(TEXT("Waypoint"));
				WorldMarker->MaxShowDistance = 999999.f;
			}
			// Update compass marker (if we are going to have a compass system)
		}
	}
	else
	{
		if (WorldMarker)
		{
			WorldMarker->Destroy();
			// Update compass marker (if we are going to have a compass system)
		}
	}
	// FHitResult Hit;
	// FCollisionObjectQueryParams Params;
	// Params.AddObjectTypesToQuery(ECC_WorldStatic);
	// Params.AddObjectTypesToQuery(ECC_WorldDynamic);
	// const FVector StartLocation = FVector(Location.X, Location.Y, Location.Z + 250000.f);
	// const FVector EndLocation = FVector(Location.X, Location.Y, Location.Z - 500000.f);
	// if (GetWorld()->LineTraceSingleByObjectType(Hit, StartLocation, EndLocation, Params))
	// {
	// 	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Black, false, 10.f, 0, 20);
	// 	
	// 	if (WorldMarker)
	// 	{
	// 		float Distance = FVector::Distance(Hit.ImpactPoint, WorldMarker->GetActorLocation());
	// 		LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("Distance is %f"), Distance);
	// 		if (Distance < 700.f)
	// 		{
	// 			WorldMarker->Destroy();
	// 			LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("Destroying WorldMarker, Distance is %f"), Distance);
	// 		}
	// 	}
	// 	
	// 	WorldMarker = GetWorld()->SpawnActor<ARPG3DWorldMarker>(MapSettings->RPG3DWorldMarker, Hit.ImpactPoint, FRotator(0.f));
	// 	if (WorldMarker)
	// 	{
	// 		WorldMarker->SetOwner(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	// 		WorldMarker->bQuestWaypoint = false;
	// 		WorldMarker->WaypointName = FText::FromString(TEXT("Waypoint"));
	// 		WorldMarker->MaxShowDistance = 999999.f;
	// 	}
	// 	// Update compass marker (if we are going to have a compass system)
	// }
}

void URPGMapSubsystem::InitializeMapScreen()
{
	const UMapSubsystemSettings* MapSettings = UMapSubsystemSettings::Get();
	check(MapSettings);

	if (!MapSettings->MapScreenWidgetClass)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("MapScreenWidgetClass is not set in project settings"));
		return;
	}

	MapScreenWidget = Cast<URPGMapScreenWidget>(CreateWidget(GetWorld(), MapSettings->MapScreenWidgetClass));
	if (MapScreenWidget)
	{
		// Need to call this explicitly for map screen POIs to creat properly
		MapScreenWidget->NativeConstruct();
		MapScreenWidget->SetVisibility(ESlateVisibility::Collapsed);

		LOG_WITH_FUNCTION_NAME(LogRPGMap, Verbose, TEXT("Map Screen initialized. Pointer: %s"), *MapScreenWidget->GetName());
	}
}

void URPGMapSubsystem::ToggleMapScreen()
{
	ARPGPlayer_Controller* RPGPlayerController = Cast<ARPGPlayer_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ensure(RPGPlayerController);

	if (MapScreenWidget)
	{
		if (MapScreenWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			// We only run this logic once so we just might set initial owning player here too (since we need to do that once as well)
			if (!MapScreenWidget->IsInViewport())
			{
				MapScreenWidget->AddToViewport();
				MapScreenWidget->SetOwningPlayer(RPGPlayerController); // Explicitly set the owning player since the widget is created before the player controller is initialized hence owning player is null (we need this for focus)
			}
			
			MapScreenWidget->SetVisibility(ESlateVisibility::Visible);
			MapScreenWidget->SetFocus();
			
			RPGPlayerController->SetUIInputMode();
		}
		else
		{
			MapScreenWidget->SetVisibility(ESlateVisibility::Collapsed);
			
			RPGPlayerController->SetDefaultInputMode();
		}
	}
}

FMapValuesTableRow* URPGMapSubsystem::GetMapValuesTableRow() const
{
	const UMapSubsystemSettings* MapSettings = UMapSubsystemSettings::Get();
	check(MapSettings);
	if (!MapSettings->MapDataTable)
	{
		if (!MapSettings->MapDataTable.LoadSynchronous())
		{
			LOG_WITH_FUNCTION_NAME(LogRPGMap, Error, TEXT("MapDataTable couldn't be loaded, probably it is null"));
			return nullptr;
		}
		LOG_WITH_FUNCTION_NAME(LogRPGMap, Warning, TEXT("MapDataTable loaded"));
	}
	
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	FMapValuesTableRow* MapRow = MapSettings->MapDataTable->FindRow<FMapValuesTableRow>(*LevelName, TEXT("Map Table Context"));
	return MapRow;
}

void URPGMapSubsystem::UpdateMapDimensions()
{
	const FMapValuesTableRow* MapRow = GetMapValuesTableRow();
	if (!MapRow)
	{
		return;
	}

	const UMapSubsystemSettings* MapSettings = UMapSubsystemSettings::Get();
	check(MapSettings);

	MapDimensions.MapXDiv = MapRow->MapSizeX / MapSettings->WidgetMapSize;
	MapDimensions.MapYDiv = MapRow->MapSizeY / MapSettings->WidgetMapSize;
	MapDimensions.MapXOffset = MapRow->MapOffsetX / MapDimensions.MapXDiv;
	MapDimensions.MapYOffset = MapRow->MapOffsetY / MapDimensions.MapYDiv;
}

FMapDimensions URPGMapSubsystem::GetMapDimensions() const
{
	return MapDimensions;
}
