// Destruction Games. 2022


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
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera 
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SetRootComponent(CameraComponent);

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

	// Floating movement component
	FloatingMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovementComp"));

	// Scene capture component
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;

	ScreenshotRenderTarget = nullptr;
}

// Called when the game starts or when spawned
void APhotoModePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhotoModePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APhotoModePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent)
	{	
		PlayerInputComponent->BindAction("CaptureScreenshot", IE_Pressed, this, &APhotoModePawn::CaptureScreenshot);

		PlayerInputComponent->BindAxis("MoveForward", this, &APhotoModePawn::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &APhotoModePawn::MoveRight);
		PlayerInputComponent->BindAxis("LookUp", this, &APhotoModePawn::LookUp);
		PlayerInputComponent->BindAxis("Turn", this, &APhotoModePawn::Turn);

	}
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
