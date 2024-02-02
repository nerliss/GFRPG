// Oleksandr Tkachov 2022-2024


#include "Characters/PhotoModePawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Utility/Utility.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
APhotoModePawn::APhotoModePawn()
{
	PrimaryActorTick.bCanEverTick = false;

	// Camera 
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SetRootComponent(CameraComponent);

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

	// Floating movement component
	FloatingMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovementComp"));
	FloatingMovementComponent->UpdatedComponent = CameraComponent;

	// Scene capture component
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;

	ScreenshotRenderTarget = nullptr;
}

void APhotoModePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("CaptureScreenshot", IE_Pressed, this, &APhotoModePawn::CaptureScreenshot);

	PlayerInputComponent->BindAxis("MoveForward", this, &APhotoModePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APhotoModePawn::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APhotoModePawn::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APhotoModePawn::Turn);
}

void APhotoModePawn::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(CameraComponent->GetForwardVector(), Value);
	}
}

void APhotoModePawn::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(CameraComponent->GetRightVector(), Value);
	}
}

void APhotoModePawn::LookUp(float Value)
{
	if (Value != 0.f)
	{
		AddControllerPitchInput(Value);
	}
}

void APhotoModePawn::Turn(float Value)
{
	if (Value != 0.f)
	{
		AddControllerYawInput(Value);
	}
}

void APhotoModePawn::CaptureScreenshot()
{
	SceneCaptureComponent->TextureTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1920, 1080);
	
	SceneCaptureComponent->CaptureScene();
	DEBUGMESSAGE(3.f, FColor::Green, "Screenshot was captured");
}

