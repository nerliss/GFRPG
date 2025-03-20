// Oleksandr Tkachov 2022-2025


#include "Interactable/RPGLockpickingMinigameActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Widgets/LockpickingMinigame/RPGLockpickingMinigameWidget.h"

ARPGLockpickingMinigameActor::ARPGLockpickingMinigameActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	LockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockMesh"));

	LeftPickScene = CreateDefaultSubobject<USceneComponent>(TEXT("LeftPickScene"));

	RightPickScene = CreateDefaultSubobject<USceneComponent>(TEXT("RightPickScene"));

	LeftPickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPickMesh"));
	LeftPickMesh->SetupAttachment(LeftPickScene);

	RightPickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPickMesh"));
	RightPickMesh->SetupAttachment(RightPickScene);

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));

	LockpickingMinigameWidgetClass = nullptr;

}

void ARPGLockpickingMinigameActor::BeginPlay()
{
	Super::BeginPlay();

	// Render only this actor
	SceneCaptureComponent->ShowOnlyActorComponents(this);

	auto* LockpickingMinigameWidget = Cast<URPGLockpickingMinigameWidget>(CreateWidget(GetWorld(), LockpickingMinigameWidgetClass));
	if (LockpickingMinigameWidget)
	{
		LockpickingMinigameWidget->AddToViewport();
	}
}

void ARPGLockpickingMinigameActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

