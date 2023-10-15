// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PhotoModePawn.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

UCLASS(hidecategories = ("Replication"))
class INVENTORYPROJECTV3_API APhotoModePawn : public APawn
{
	GENERATED_BODY()

public:
	
	APhotoModePawn();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/************************************************************************/
	/*                           Photo mode pawn                            */
	/************************************************************************/

	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void Turn(float Value);

	void CaptureScreenshot();

protected:

	/* Pawn's camera */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	/* Pawn's movement */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UFloatingPawnMovement* FloatingMovementComponent;

	/* Scene capture component. Used for taking screenshots */
	UPROPERTY(EditDefaultsOnly, Category = "PhotoMode")
	USceneCaptureComponent2D* SceneCaptureComponent;

	/* Render target to save created screenshot to */
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Photo mode")*/
	UPROPERTY(EditDefaultsOnly, Category = "PhotoMode")
	UTextureRenderTarget2D* ScreenshotRenderTarget;
};
