// Oleksandr Tkachov 2022-2023


#include "Containers/RPGBaseContainer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/RPGInventory_Component.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Items/RPGItem_Base.h"
#include "Utility/LogDefinitions.h"

ARPGBaseContainer::ARPGBaseContainer()
{
	PrimaryActorTick.bCanEverTick = false;

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

	FillContainerInventory();
}

void ARPGBaseContainer::InteractNative(AActor* Interactor)
{
	// MyTODO: Replace this with IsChildOf or something since casting a type without using it confuses
	const auto* Player = Cast<ARPGPlayerCharacter>(Interactor);
	if (!Player)
	{
		UE_LOG(LogRPGInventory, Error, TEXT("[ARPGBaseContainer::FillContainerInventory] Interactor is not a player"));
		return;
	}

	// Open this container's inventory if interactor is player
	ContainerInventoryComp->ToggleInventory();
}

FText ARPGBaseContainer::GetNameNative() const
{
	return ContainerInventoryComp->Name;
}

void ARPGBaseContainer::FillContainerInventory()
{
	// MyTODO: Add some conditions to check and display that the item is stackable or not, if yes - strip the quantity to 1 / or split into several different stacks, by one. 
	for (int i = 0; i < ItemToAdd.Num(); i++)
	{
		const auto ContainerItem = ItemToAdd[i];
		const auto* ItemCDO = ContainerItem.Item.GetDefaultObject();
		if (!ItemCDO)
		{
			UE_LOG(LogRPGInventory, Error, TEXT("[ARPGBaseContainer::FillContainerInventory] ItemCDO wasn't created."));
			continue;
		}

		if (!(ContainerInventoryComp->Inventory.IsValidIndex(i)))
		{
			UE_LOG(LogRPGInventory, Error, TEXT("[ARPGBaseContainer::FillContainerInventory] Container's inventory is overfilled and can't copy more items from ItemsToAdd."));
			return;
		}

		ContainerInventoryComp->Inventory[i].Item = ItemCDO->Item;
		ContainerInventoryComp->Inventory[i].Quantity = ContainerItem.Quantity;
	}

	UE_LOG(LogRPGInventory, Verbose, TEXT("[ARPGBaseContainer::FillContainerInventory] Finished filling %s inventory."), *GetHumanReadableName());
}

