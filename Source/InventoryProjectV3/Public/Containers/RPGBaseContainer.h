// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/RPGInventoryConfig.h"
#include "Interfaces/RPGInteractInterface.h"
#include "RPGBaseContainer.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class URPGInventoryComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPGBaseContainer : public AActor, public IRPGInteractInterface
{
	GENERATED_BODY()
	
public:	

	ARPGBaseContainer();
	
	virtual void BeginPlay() override;
#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/* IRPGInteractInterface */
	virtual void InteractNative(AActor* Interactor) override;
	virtual FText GetNameNative() const override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ContainerMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* TraceSphere;

	/* Inventory component stores data about items */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	URPGInventoryComponent* ContainerInventoryComp;

	/* Items to fill this container's inventory with. Use this to set what items should be in this container. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FContainerItem> ItemToAdd;

private:

	void FillContainerInventory();
};
