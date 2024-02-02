// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGMapPOIWidget.generated.h"

class UImage;

/**
 * Map Point of Interest widget base class
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGMapPOIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* POIImage;

	AActor* Owner;

	float MapWidgetRadius;

	float FindAngleDegrees(FVector2D A, FVector2D B) const;
	FVector2D FindCoordinates(float Radius, float Degrees) const;
	void SetPOIIcon();

};
