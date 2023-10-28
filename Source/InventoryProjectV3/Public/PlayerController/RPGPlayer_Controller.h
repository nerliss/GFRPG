// Oleksandr Tkachov 2022-2023

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

	ARPGPlayer_Controller();

public:

	virtual void BeginPlay() override;

	/* Player controller specific input definitions */
	virtual void SetupInputComponent() override;

	/* HUD class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD Widget")
	TSubclassOf<UUserWidget> MainHUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD Widget")
	URPGHUD_Widget* MainHUDWidget;

	/************************************************************************/
	/*                              Photo mode                              */
	/************************************************************************/

	/* Photo mode pawn to spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PhotoMode")
	TSubclassOf<APhotoModePawn> PhotoModePawnClass;
	
	UPROPERTY()
	APhotoModePawn* PhotoModePawn;

	UPROPERTY()
	APawn* OriginalPlayerPawn;

	void SpawnPhotoModePawn();
	void DespawnPhotoModePawn();

	/* Spawns and possesses photo mode pawn if not in photo mode already, despawns and unpossesses otherwise */
	UFUNCTION(BlueprintCallable, Category = "PhotoMode")
	void TogglePhotoMode();

};
