// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Map/RPG3DWorldMarker.h"
#include "Utility/LogDefinitions.h"
#include "Map/RPGMapIconComponent.h"
#include "Utility/Utility.h"

void URPGMapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	On3DWorldMarkerSpawned.AddUObject(this, &URPGMapSubsystem::Spawn3DWorldMarker);
}

void URPGMapSubsystem::RegisterIcon(URPGMapIconComponent* Icon)
{
	UE_LOG(LogRPGMap, Verbose, TEXT("[URPGMapSubsystem::RegisterIcon] Adding icon %s"), *GetNameSafe(Icon));
	Icons.AddUnique(Icon);
}

void URPGMapSubsystem::UnregisterIcon(URPGMapIconComponent* Icon)
{
	UE_LOG(LogRPGMap, Verbose, TEXT("[URPGMapSubsystem::UnregisterIcon] Removing icon %s"), *GetNameSafe(Icon));
	Icons.Remove(Icon);
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
}
