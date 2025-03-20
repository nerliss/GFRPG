// Oleksandr Tkachov 2022-2025


#include "Interactable/RPGDoorBase.h"

#include "Components/SphereComponent.h"
#include "Interactable/RPGLockpickingMinigameActor.h"
#include "Utility/Utility.h"

ARPGDoorBase::ARPGDoorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	FrameMesh->SetupAttachment(RootComponent);
	FrameMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FrameMesh->SetCollisionResponseToAllChannels(ECR_Block);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorMesh->SetCollisionResponseToAllChannels(ECR_Block);

	TraceSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TraceSphere"));
	TraceSphere->SetupAttachment(RootComponent);
	TraceSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TraceSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TraceSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	TraceSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	DoorName = FText::FromString(TEXT("Basic Door"));

	bClosed = true;

	LockpickingMinigameClass = nullptr;
}

void ARPGDoorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGDoorBase::InteractNative(AActor* Interactor)
{
	// 1. Check if locked
	// 2. Start lockpicking minigame
	// 3. Open the door otherwise
	if (bClosed)
	{
		const FTransform SpawnTransform = FTransform(FVector(0.f, 0.f, -2000.f));

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		auto* LockpickingActor = GetWorld()->SpawnActor<ARPGLockpickingMinigameActor>(LockpickingMinigameClass, SpawnTransform, Params);
		if (LockpickingActor)
		{
			DEBUGMESSAGE(3.f, FColor::Green, "Lockpicking actor spawned");
		}
		else
		{
			DEBUGMESSAGE(3.f, FColor::Red, "Lockpicking actor spawn failed probably because LockPickingMinigameClass is null, please set it to a valid class in Door Actor");
		}

		OpenDoor();
	}
	else
	{
		CloseDoor();
	}
}

FText ARPGDoorBase::GetNameNative() const
{
	return DoorName;
}

void ARPGDoorBase::OpenDoor()
{
	// Open the door
	DEBUGMESSAGE(3.f, FColor::Green, "Door open");
	bClosed = false;

	// Temp
	//DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DoorMesh->SetHiddenInGame(true);
}

void ARPGDoorBase::CloseDoor()
{
	// Close the door
	DEBUGMESSAGE(3.f, FColor::Red, "Door closed");
	bClosed = true;

	// Temp
	//DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorMesh->SetHiddenInGame(false);
}

