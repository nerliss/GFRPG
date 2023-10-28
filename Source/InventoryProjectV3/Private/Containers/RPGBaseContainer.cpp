// Oleksandr Tkachov 2022-2023


#include "Containers/RPGBaseContainer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/RPGInventory_Component.h"
#include "Characters/RPGPlayerCharacter.h"


ARPGBaseContainer::ARPGBaseContainer()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	ContainerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContainerMesh"));
	ContainerMesh->SetupAttachment(RootComponent);
	ContainerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ContainerMesh->SetCollisionResponseToAllChannels(ECR_Block);

	TraceSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TraceSphere"));
	TraceSphere->SetupAttachment(RootComponent);
	TraceSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TraceSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TraceSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);

	ContainerInventoryComp = CreateDefaultSubobject<URPGInventory_Component>(TEXT("ContainerInventoryComp"));
	ContainerInventoryComp->Name = FText::FromString("Base Chest");
	ContainerInventoryComp->SlotsNumber = 4;
}

void ARPGBaseContainer::BeginPlay()
{
	Super::BeginPlay();

	ContainerInventoryComp->Inventory = Items;
	/*for (const auto Item : Items)
	{
		InventoryComp->Inventory = Items;
	}*/
}

void ARPGBaseContainer::InteractNative(AActor* Interactor)
{
	// MyTODO: Replace this with IsChildOf or something since casting a type without using it confuses
	const auto* Player = Cast<ARPGPlayerCharacter>(Interactor);
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Interactor is not a player"));
		return;
	}

	// Open this container's inventory if interactor is player
	ContainerInventoryComp->ToggleInventory();
	UE_LOG(LogTemp, Log, TEXT("Interacted with container, its inventory window should be opened"));
}

FText ARPGBaseContainer::GetNameNative() const
{
	return ContainerInventoryComp->Name;
}

