// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG3DWorldMarkerWidget.generated.h"

class UBorder;
class UTextBlock;
/**
 * 3D World Marker Widget class. This widget is used in 3DWorldMarker class to display marker-related information in 3D space.
 */
UCLASS()
class INVENTORYPROJECTV3_API URPG3DWorldMarkerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* MarkerText = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* DistanceText = nullptr;

	UPROPERTY(meta=(BindWidgetOptional))
	UBorder* MarkerBorder = nullptr;

public:
	
	void UpdateDistance(FText DistanceAsText, bool bMapMarker, float PlayerDistance, float MaxDistance);
	void UpdateName(FText Name);
};
