// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGMapPlayerIconWidget.generated.h"

class UImage;

/**
 * Map Player Icon base class
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGMapPlayerIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* PlayerIcon;

private:

	void UpdateIconRotation();
};
