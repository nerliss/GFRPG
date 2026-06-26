// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPGHUDMain.generated.h"

/**
 * Base HUD class
 */
UCLASS()
class INVENTORYPROJECTV3_API ARPGHUDMain : public AHUD
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
