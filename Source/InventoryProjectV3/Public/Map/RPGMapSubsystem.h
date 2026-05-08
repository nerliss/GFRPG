// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGMapSubsystem.generated.h"

class URPGMapPOIWidget;
class URPGMapScreenWidget;
class ARPGMapBoundsVolume;
class URPGMapIconComponent;
class ARPG3DWorldMarker;
class URPGMapWidgetBase;

USTRUCT(Blueprintable)
struct FMapDimensions
{
	GENERATED_BODY()

	float MapXDiv;

	float MapYDiv;

	float MapXOffset;

	float MapYOffset;
};

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
	TSoftObjectPtr<UDataTable> MapDataTable;
	
	UPROPERTY(EditAnywhere, Config, Category = "Settings")
	TSubclassOf<ARPG3DWorldMarker> RPG3DWorldMarker;

	UPROPERTY(EditAnywhere, Config, Category = "Settings")
	TSubclassOf<URPGMapPOIWidget> POIWidgetClass;
	
	UPROPERTY(EditAnywhere, Config, Category = "Settings")
	TSubclassOf<URPGMapScreenWidget> MapScreenWidgetClass;

	// TODO: Find a better way to match widget's actual size and this
	/* Map widget size. Used in map calculations. The widget's size MUST match */
	UPROPERTY(EditAnywhere, Config, Category = "Settings")
	float WidgetMapSize = 1000.f;

	/* Screenshot resolution for Map Image Generator. It is advised to not use huge number like 10k+ */
	UPROPERTY(EditAnywhere, Config, Category = "Settings")
	FVector2D MapScreenshotResolution = FVector2D(4000, 4000);
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWorldMarkerToggled, bool /*bSpawn*/, FVector2D /*MarkerMapLocation*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOn3DWorldMarkerSpawned, bool /*bSpawn*/, FVector /*MarkerWorldLocation*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMapZoomChanged, float /*Zoom*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPointOfInterestComponentDestroyed, AActor* /* Owner */, URPGMapWidgetBase* /* MapReference */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPointOfInterestIconSpawned, URPGMapPOIWidget* /* Icon */);

UCLASS()
class INVENTORYPROJECTV3_API URPGMapSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Paint a soft reveal circle into FogRT at World location (optional). */
	UFUNCTION(BlueprintCallable, Category="Map|Fog")
	void RevealAtWorld(const FVector& World, float RadiusWorldUnits = 600.f, float Opacity = 1.f);
	
	UFUNCTION()
	void Spawn3DWorldMarker(bool bSpawn, FVector Location);

	void InitializeMapScreen();
	void ToggleMapScreen();
	
	FMapValuesTableRow* GetMapValuesTableRow() const;
	
	FMapDimensions GetMapDimensions() const;
	
	/** Static world map image (or set at runtime) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	TSoftObjectPtr<UTexture2D> MapTexture;

	/** Optional fog-of-war render target. If null, fog is disabled */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|Fog")
	UTextureRenderTarget2D* FogRT = nullptr;

	FOnWorldMarkerToggled OnWorldMarkerToggled;
	FOn3DWorldMarkerSpawned On3DWorldMarkerSpawned;
	FOnMapZoomChanged OnMapZoomChanged;
	FOnPointOfInterestComponentDestroyed OnPointOfInterestComponentDestroyed;
	FOnPointOfInterestIconSpawned OnPointOfInterestIconSpawned;

	UPROPERTY()
	ARPG3DWorldMarker* WorldMarker;

	UPROPERTY()
	URPGMapScreenWidget* MapScreenWidget;

	FMapDimensions MapDimensions;
	
private:

	void UpdateMapDimensions();
	
};
