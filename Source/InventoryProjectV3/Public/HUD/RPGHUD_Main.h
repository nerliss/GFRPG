// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPGHUD_Main.generated.h"

/**
 * Base HUD class
 */
UCLASS()
class INVENTORYPROJECTV3_API ARPGHUD_Main : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
	bool bShowCrosshair = true;

	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	UTexture* CrosshairTexture;

	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	void DrawCrosshair(float ScreenWidth, float ScreenHeight);
	
};
