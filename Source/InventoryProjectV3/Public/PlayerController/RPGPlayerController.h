// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

class URPGHUDWidget;
class APhotoModePawn;

UCLASS()
class INVENTORYPROJECTV3_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	ARPGPlayerController();

	UFUNCTION(BlueprintCallable)
	URPGHUDWidget* GetHUDWidget() const;

	/* Sets input mode game only and hides cursor */
	void SetDefaultInputMode();

	void SetUIInputMode();
	
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void SpawnPhotoModePawn();
	void DespawnPhotoModePawn();

	UFUNCTION(BlueprintCallable, Category = "PhotoMode")
	void TogglePhotoMode();
	
	// TODO: Move to a config or something
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD Widget")
	TSubclassOf<UUserWidget> MainHUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD Widget")
	URPGHUDWidget* MainHUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhotoMode")
	TSubclassOf<APhotoModePawn> PhotoModePawnClass;
	
	UPROPERTY()
	APhotoModePawn* PhotoModePawn;

	UPROPERTY()
	APawn* OriginalPlayerPawn;
	
private:

	/* Create HUD widget */
	void InitializeHUDWidget();
};
