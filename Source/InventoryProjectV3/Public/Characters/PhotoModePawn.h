// Destruction Games. 2022

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

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/************************************************************************/
	/*                           Photo mode pawn                            */
	/************************************************************************/

	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void Turn(float Value);

	void CaptureScreenshot();

private:

	/* Pawn's camera */
	UCameraComponent* CameraComponent;

	/* Pawn's movement */
	UFloatingPawnMovement* FloatingMovementComponent;

	/* Scene capture component. Used for taking screenshots */
	USceneCaptureComponent2D* SceneCaptureComponent;
	
protected:

	/* Render target to save created screenshot to */
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Photo mode")*/
	UTextureRenderTarget2D* ScreenshotRenderTarget;
};
