// Destruction Games. 2022


#include "Items/RPGItem_Base.h"
#include "Components/RPGInventory_Component.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"

DEFINE_LOG_CATEGORY(LogRPGItem);

ARPGItem_Base::ARPGItem_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup root component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);

	// Setup sphere collision component
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);

	// Setup skeletal mesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECR_Block);

	// Setup static mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void ARPGItem_Base::InteractNative(AActor* Interactor)
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

FText ARPGItem_Base::GetNameNative() const
{
	return Item.Name;
}

int32 ARPGItem_Base::GetQuantity_Implementation() const
{
	return 1; // Base class, should be overriden in all child classes to give a custom amount
}

void ARPGItem_Base::AddToInventory(AActor* Interactor, int32 QuantityOfItemsToAdd)
{
	if (!Interactor)
	{
		// Interactor is invalid
		return;
	}

	auto* InventoryComp = Cast<URPGInventory_Component>(Interactor->GetComponentByClass(URPGInventory_Component::StaticClass()));
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
