// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGMapWidgetBase.generated.h"

class UImage;
class UCanvasPanel;
class URPGMapSubsystem;
class URPGMapIconComponent;

UCLASS()
class INVENTORYPROJECTV3_API URPGMapWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta=(BindWidget))
	UImage* MapImage = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* IconCanvas = nullptr;

	/** Optional: assign in BP if you want to display FogRT overlay via material. */
	UPROPERTY(meta=(BindWidgetOptional))
	UImage* FogImage = nullptr;

	/** If true, rotate the whole map by player yaw (minimap style). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	bool bRotateWithPlayer = false;

	/** Scale of the map image. 1.0 fits to its brush size; >1 zooms in. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map")
	float Zoom = 1.0f;

protected:

	/** Loads/sets the map image brush. */
	void RefreshMapBrush();

	/** Simple icon rebuild each tick (you can pool later). */
	void RefreshIcons();

	/** For minimap center-on-player translation (override in a child if needed). */
	virtual bool ComputeMapTranslation(FVector2D& OutTranslation, float& OutRotationDeg) const;

	TWeakObjectPtr<URPGMapSubsystem> MapSubsystem;
};
