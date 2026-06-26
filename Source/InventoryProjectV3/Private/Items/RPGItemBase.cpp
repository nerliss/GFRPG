// Oleksandr Tkachov 2021-2026


#include "Items/RPGItemBase.h"
#include "Components/RPGInventoryComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Utility/LogDefinitions.h"

ARPGItemBase::ARPGItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	// Setup sphere collision component
	TraceSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TraceSphere"));
	TraceSphere->SetupAttachment(RootComponent);
	TraceSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TraceSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TraceSphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	TraceSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	// Setup skeletal mesh
	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMesh"));
	ItemSkeletalMesh->SetupAttachment(RootComponent);
	ItemSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemSkeletalMesh->SetCollisionResponseToAllChannels(ECR_Block);

	// Setup static mesh
	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemStaticMesh->SetupAttachment(RootComponent);
	ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemStaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void ARPGItemBase::InteractNative(AActor* Interactor)
{
	if (!Interactor)
	{
		// Interactor is invalid
		return;
	}

	const bool bQuestItem = Item.bQuestItem;
	if (bQuestItem)
	{
		// MyTODO: Check if player has a quest related to this item and only then add it to inventory (Quest system is NYI)
		UE_LOG(LogRPGItem, Error, TEXT("Quest items are not supported yet."));
		return;
	}

	AddToInventory(Interactor, GetQuantity()); 
}

FText ARPGItemBase::GetNameNative() const
{
	return Item.Name;
}

int32 ARPGItemBase::GetQuantity_Implementation() const
{
	return 1; // Base class, should be overriden in all child classes to give a custom amount
}

bool ARPGItemBase::UseItem_Implementation() const
{
	return true; // Base class, should be overriden in all child classes to give a custom functionality
}

void ARPGItemBase::AddToInventory(AActor* Interactor, int32 QuantityOfItemsToAdd)
{
	if (!Interactor)
	{
		// Interactor is invalid
		return;
	}

	auto* InventoryComp = Cast<URPGInventoryComponent>(Interactor->GetComponentByClass(URPGInventoryComponent::StaticClass()));
	if (!InventoryComp)
	{
		// Interactor does not have an inventory component
		return;
	}

	FInventorySlot NewSlot;
	NewSlot.Item = Item;
	NewSlot.Quantity = QuantityOfItemsToAdd;

	const bool bSuccessfullyAdded = InventoryComp->AddToInventory(NewSlot);
	if (!bSuccessfullyAdded)
	{
		UE_LOG(LogRPGItem, Error, TEXT("[RPGItem_Base::AddToInventory] Failed to add %s item to inventory!"), *NewSlot.Item.Name.ToString());
		return;
	}

	// MyTODO: Update quest objectives (Quest system C++ is NYI), add respawn functionality
	UE_LOG(LogRPGItem, Log, TEXT("[RPGItem_Base::AddToInventory] Successfully added %i item %s."), QuantityOfItemsToAdd, *Item.Name.ToString());
	Destroy();
}
