// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGMapSubsystem.generated.h"

class ARPGMapBoundsVolume;
class URPGMapComponent;

UCLASS()
class INVENTORYPROJECTV3_API URPGMapSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	/** Static world map image (or set at runtime) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	TSoftObjectPtr<UTexture2D> MapTexture;

	/** If your map image is flipped vertically vs world Y */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	bool bFlipY = false;

	/** Optional fog-of-war render target. If null, fog is disabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|Fog")
	UTextureRenderTarget2D* FogRT = nullptr;

	/** Active bounds volume (set from level/game mode). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map")
	TWeakObjectPtr<ARPGMapBoundsVolume> BoundsVolume;

	/** World icons registered by components */
	const TArray<TWeakObjectPtr<URPGMapComponent>>& GetIcons() const { return Icons; }
	
	UFUNCTION(BlueprintCallable, Category="Map")
	void SetBoundsVolume(ARPGMapBoundsVolume* Volume);

	UFUNCTION(BlueprintCallable, Category="Map")
	bool WorldToMapUV(const FVector& World, FVector2D& OutUV) const;

	UFUNCTION(BlueprintCallable, Category="Map")
	FVector MapUVToWorld(const FVector2D& UV, float Z = 0.f) const;

	void RegisterIcon(URPGMapComponent* Icon);
	void UnregisterIcon(URPGMapComponent* Icon);

	/** Paint a soft reveal circle into FogRT at World location (optional). */
	UFUNCTION(BlueprintCallable, Category="Map|Fog")
	void RevealAtWorld(const FVector& World, float RadiusWorldUnits = 600.f, float Opacity = 1.f);

private:
	
	// UPROPERTY(Transient)
	TArray<TWeakObjectPtr<URPGMapComponent>> Icons;
};
