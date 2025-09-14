// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapSubsystem.h"

#include "EngineUtils.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Map/RPGMapBoundsVolume.h"
#include "Utility/LogDefinitions.h"
#include "Map/RPGMapIconComponent.h"

void URPGMapSubsystem::SetBoundsVolume(ARPGMapBoundsVolume* Volume)
{
	BoundsVolume = Volume;
}

bool URPGMapSubsystem::WorldToMapUV(const FVector& World, FVector2D& OutUV) const
{
	if (!BoundsVolume.IsValid())
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMapSubsystem::WorldToMapUV] BoundsVolume is not valid"));
		return false;
	}

	FVector2D Min, Max;
	BoundsVolume->GetXYMinMax(Min, Max);
	const FVector2D Size = Max - Min;
	if (Size.X <= 0.f || Size.Y <= 0.f)
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMapSubsystem::WorldToMapUV] Map size is invalid"));
		return false;
	}

	const FVector2D WorldSize = FVector2D(World.X, World.Y);
	OutUV.X = (WorldSize.X - Min.X) / Size.X;
	OutUV.Y = (WorldSize.Y - Min.Y) / Size.Y;

	if (bFlipY)
	{
		OutUV.Y = 1.f - OutUV.Y;
		UE_LOG(LogRPGMap, Log, TEXT("[URPGMapSubsystem::WorldToMapUV] Flipped Y coords (%f). New OutUV vector is %s"), OutUV.Y, *OutUV.ToString());
	}

	UE_LOG(LogRPGMap, Log, TEXT("[URPGMapSubsystem::WorldToMapUV] OutUV is %s"), *OutUV.ToString());
	return true;
}

FVector URPGMapSubsystem::MapUVToWorld(const FVector2D& UV, float Z) const
{
	if (!BoundsVolume.IsValid())
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMapSubsystem::MapUVToWorld] BoundsVolume is not valid"));
		return FVector::ZeroVector;
	}

	FVector2D Min, Max;
	BoundsVolume->GetXYMinMax(Min, Max);
	const FVector2D Size = Max - Min;
	if (Size.X <= 0.f || Size.Y <= 0.f)
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMapSubsystem::MapUVToWorld] Map size is invalid"));
		return FVector::ZeroVector;
	}

	FVector2D UseUV = UV;
	if (bFlipY)
	{
		UseUV.Y = 1.f - UseUV.Y;
		UE_LOG(LogRPGMap, Log, TEXT("[URPGMapSubsystem::MapUVToWorld] Flipped Y coords (%f). New UseUV vector is %s"), UseUV.Y, *UseUV.ToString());
	}

	const FVector2D XY = Min + FVector2D(UseUV.X * Size.X, UseUV.Y * Size.Y);
	const FVector WorldCoords = FVector(XY.X, XY.Y, Z);

	UE_LOG(LogRPGMap, Log, TEXT("[URPGMapSubsystem::MapUVToWorld] World coords are %s"), *WorldCoords.ToString());
	return WorldCoords;
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

	FVector2D UV;
	if (!WorldToMapUV(World, UV))
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMapSubsystem::RevealAtWorld] WorldToMapUV returned false"));
		return;
	}

	// Convert world radius to UV radius using bounds max dimension
	FVector2D Min, Max;
	BoundsVolume->GetXYMinMax(Min, Max);
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

bool URPGMapSubsystem::FindAndSetBoundsByActorTag(FName Tag)
{
	for (TActorIterator<ARPGMapBoundsVolume> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag(Tag))
		{
			UE_LOG(LogRPGMap, Verbose, TEXT("[URPGMapSubsystem::FindAndSetBoundsByActorTag] Found Bounds Volume with tag %s: %s"), *Tag.ToString(), *It->GetName())
			SetBoundsVolume(*It);
			return true;
		}
	}
	
	return false;
}
