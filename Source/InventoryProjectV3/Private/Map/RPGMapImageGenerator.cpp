// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapImageGenerator.h"

#include "AutomationBlueprintFunctionLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

ARPGMapImageGenerator::ARPGMapImageGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TopLeftMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopLeftMarker"));
	TopLeftMarker->SetupAttachment(Root);

	TopRightMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopRightMarker"));
	TopRightMarker->SetupAttachment(Root);

	BottomLeftMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomLeftMarker"));
	BottomLeftMarker->SetupAttachment(Root);

	BottomRightMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomRightMarker"));
	BottomRightMarker->SetupAttachment(Root);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(Root);
	Spline->SetClosedLoop(true);

	InstancedSM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	InstancedSM->SetupAttachment(Root);
	
	MapSize = 1000.f;
	CameraLocation = FVector(0, 0, 0);
	BorderScale = FVector(1, 1, 1);
}

void ARPGMapImageGenerator::BeginPlay()
{
	Super::BeginPlay();

	TakeScreenshot();
}

void ARPGMapImageGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	
}

void ARPGMapImageGenerator::TakeScreenshot()
{
#if WITH_EDITOR
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	// Delay 1
	const FRotator CameraRotation = FRotator(-90.f, -90.f, 0.f);
	ACameraActor* SpawnedCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation);
	// Delay 1
	if (SpawnedCamera)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			PC->SetViewTarget(SpawnedCamera);
			// Delay 1

			UAutomationBlueprintFunctionLibrary::TakeHighResScreenshot(1920, 1080, "", SpawnedCamera);
			// Delay 0.2
			SpawnedCamera->Destroy();
		}
	}
#endif
}
