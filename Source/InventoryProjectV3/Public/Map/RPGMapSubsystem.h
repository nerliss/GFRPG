// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGMapSubsystem.generated.h"

class ARPGMapBoundsVolume;
class URPGMapIconComponent;
class ARPG3DWorldMarker;

USTRUCT()
struct FMapValuesTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	float MapSizeX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	float MapSizeY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	float MapOffsetX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	float MapOffsetY;

	/* Map texture. If bUseSeparateTextureForMinimap is true, this texture is only used for Map Screen Widget. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UTexture2D* MapTexture;

	/* Should Minimap Widget use a different map texture? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	bool bUseSeparateTextureForMinimap;

	/* Minimap texture. Usually is just a render of the level. Used only if bUseSeparateTextureForMinimap is true */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map", meta=(EditCondition = "bUseSeparateTextureForMinimap"))
	UTexture2D* MinimapTexture;
	
};

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Map Subsystem"))
class INVENTORYPROJECTV3_API UMapSubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	virtual FName GetContainerName() const override { return "Project"; }  
	virtual FName GetCategoryName()  const override { return "RPGSettings"; }  
	virtual FName GetSectionName()   const override { return "Map Subsystem"; }

	virtual FText GetSectionText() const override
	{
		return NSLOCTEXT("MapSubsystem", "SectionText", "Map Subsystem");
	}
	virtual FText GetSectionDescription() const override
	{
		return NSLOCTEXT("MapSubsystem", "SectionDesc", "Parameters that control RPG Map Subsystem behavior.");
	}

	static const UMapSubsystemSettings* Get() { return GetDefault<UMapSubsystemSettings>(); }
	static UMapSubsystemSettings* GetMutable() { return GetMutableDefault<UMapSubsystemSettings>(); }
	
	UPROPERTY(EditAnywhere, Config, Category = "Settings")
	TSubclassOf<ARPG3DWorldMarker> RPG3DWorldMarker; 
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWorldMarkerToggled, bool /*bSpawn*/, FVector2D /*MarkerMapLocation*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOn3DWorldMarkerSpawned, bool /*bSpawn*/, FVector /*MarkerWorldLocation*/);

UCLASS()
class INVENTORYPROJECTV3_API URPGMapSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable, Category="Map")
	void SetBoundsVolume(ARPGMapBoundsVolume* Volume);

	UFUNCTION(BlueprintCallable, Category="Map")
	bool WorldToMapUV(const FVector& World, FVector2D& OutUV) const;

	UFUNCTION(BlueprintCallable, Category="Map")
	FVector MapUVToWorld(const FVector2D& UV, float Z = 0.f) const;

	void RegisterIcon(URPGMapIconComponent* Icon);
	void UnregisterIcon(URPGMapIconComponent* Icon);

	/** Paint a soft reveal circle into FogRT at World location (optional). */
	UFUNCTION(BlueprintCallable, Category="Map|Fog")
	void RevealAtWorld(const FVector& World, float RadiusWorldUnits = 600.f, float Opacity = 1.f);

	UFUNCTION(BlueprintCallable, Category="Map")
	bool FindAndSetBoundsByActorTag(FName Tag);

	UFUNCTION()
	void Spawn3DWorldMarker(bool bSpawn, FVector Location);
	
	/** Static world map image (or set at runtime) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	TSoftObjectPtr<UTexture2D> MapTexture;

	/** If map image is flipped vertically vs world Y */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	bool bFlipY = false;

	/** Optional fog-of-war render target. If null, fog is disabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|Fog")
	UTextureRenderTarget2D* FogRT = nullptr;

	/** Active bounds volume (set from level/game mode). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map")
	TWeakObjectPtr<ARPGMapBoundsVolume> BoundsVolume;

	/** World icons registered by components */
	const TArray<TWeakObjectPtr<URPGMapIconComponent>>& GetIcons() const { return Icons; }

	FOnWorldMarkerToggled OnWorldMarkerToggled;
	FOn3DWorldMarkerSpawned On3DWorldMarkerSpawned;

	UPROPERTY()
	ARPG3DWorldMarker* WorldMarker;
	
private:
	
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<URPGMapIconComponent>> Icons;
};
