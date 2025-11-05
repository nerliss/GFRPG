// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGMapPOIWidget.generated.h"

class URPGMapWidgetBase;
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
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* POIImage;

	UPROPERTY(BlueprintReadOnly)
	AActor* Owner;

	UPROPERTY()
	URPGMapWidgetBase* OwningMapWidget;
	
	UFUNCTION()
	void UpdateIconSize(float ZoomFactor);

private:

	void UpdateIconVisibility();
	void UpdateIconTransform();
	void SetPOIIcon();
	void UpdateTooltipText();

	bool bIconSet;

};
