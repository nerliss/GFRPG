// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGPlayer_Controller.generated.h"

class URPGHUD_Widget;
class APhotoModePawn;

UCLASS()
class INVENTORYPROJECTV3_API ARPGPlayer_Controller : public APlayerController
{
	GENERATED_BODY()

public:
	
	ARPGPlayer_Controller();

	UFUNCTION(BlueprintCallable)
	URPGHUD_Widget* GetHUDWidget() const;

	/* Sets input mode game only and hides cursor */
	void SetDefaultInputMode();

	/* Sets input mode UI only and shows cursor */
	void SetUIInputMode();
	
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void SpawnPhotoModePawn();
	void DespawnPhotoModePawn();

	/* Spawns and possesses photo mode pawn if not in photo mode already, despawns and unpossesses otherwise */
	UFUNCTION(BlueprintCallable, Category = "PhotoMode")
	void TogglePhotoMode();
	
	// TODO: Move to a config or something
	/* HUD class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD Widget")
	TSubclassOf<UUserWidget> MainHUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD Widget")
	URPGHUD_Widget* MainHUDWidget;

	/* Photo mode pawn to spawn */
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
