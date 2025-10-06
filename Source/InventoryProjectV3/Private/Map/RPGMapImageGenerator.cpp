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
	CameraLocation = FVector(0.f, 0.f, 0.f);
	BorderScale = FVector(3.f, 3.f, 3.f);
}

void ARPGMapImageGenerator::BeginPlay()
{
	Super::BeginPlay();

	TakeScreenshot();
}

void ARPGMapImageGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const FVector TopLeftLocation = FVector((MapSize / 2.f * -1.f), MapSize / 2.f, 0.f);
	const FVector BottomLeftLocation = FVector(MapSize / 2.f, MapSize / 2.f, 0.f);
	const FVector TopRightLocation = FVector((MapSize / 2.f * -1.f), (MapSize / 2.f * -1.f), 0.f);
	const FVector BottomRightLocation = FVector(MapSize / 2.f, (MapSize / 2.f * -1.f), 0.f);
	
	TopLeftMarker->SetRelativeLocation(TopLeftLocation, false, nullptr, ETeleportType::ResetPhysics);
	BottomLeftMarker->SetRelativeLocation(BottomLeftLocation, false, nullptr, ETeleportType::ResetPhysics);
	TopRightMarker->SetRelativeLocation(TopRightLocation, false, nullptr, ETeleportType::ResetPhysics);
	BottomRightMarker->SetRelativeLocation(BottomRightLocation, false, nullptr, ETeleportType::ResetPhysics);

	const TArray<FVector> SplinePoints = {TopLeftLocation, TopRightLocation, BottomRightLocation, BottomLeftLocation};
	Spline->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::Local);

	const FVector CameraLocationTempVector = (TopLeftMarker->GetComponentLocation() + BottomLeftMarker->GetComponentLocation() + TopRightMarker->GetComponentLocation() + BottomRightMarker->GetComponentLocation()) / 4.f; // Center of square
	CameraLocation = FVector(CameraLocationTempVector.X, CameraLocationTempVector.Y, CameraLocationTempVector.Z + MapSize);

	for (int i = 0; i < Spline->GetNumberOfSplinePoints(); i++)
	{
		Spline->SetSplinePointType(i, ESplinePointType::Linear);
	}

	const int32 InstancedMeshesNumber = FMath::TruncToInt(MapSize * 4.f / 100.f);
	
	InstancedSM->ClearInstances();
	for (int i = 0; i < InstancedMeshesNumber; i++)
	{
		FVector LocationAtDistanceAlongSplinePoint = Spline->GetLocationAtDistanceAlongSpline((i * MapSize / 100.f), ESplineCoordinateSpace::Local);
		FTransform InstanceTransform;
		InstanceTransform.SetLocation(LocationAtDistanceAlongSplinePoint);
		InstanceTransform.SetScale3D(BorderScale);
		
		InstancedSM->AddInstance(InstanceTransform);
	}
}

void ARPGMapImageGenerator::TakeScreenshot()
{
#if WITH_EDITOR
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
	
	const FRotator CameraRotation = FRotator(-90.f, -90.f, 0.f);
	ACameraActor* SpawnedCamera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation);
	if (SpawnedCamera)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			PC->SetViewTarget(SpawnedCamera);

			UAutomationBlueprintFunctionLibrary::TakeHighResScreenshot(1920, 1080, "", SpawnedCamera);
			
			SpawnedCamera->Destroy();
		}
	}
#endif
}
