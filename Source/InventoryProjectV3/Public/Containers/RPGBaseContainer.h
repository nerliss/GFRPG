// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/RPGInventoryConfig.h"
#include "Interfaces/RPGInteract_Interface.h"
#include "RPGBaseContainer.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class URPGInventory_Component;

UCLASS()
class INVENTORYPROJECTV3_API ARPGBaseContainer : public AActor, public IRPGInteract_Interface
{
	GENERATED_BODY()
	
public:	

	ARPGBaseContainer();

protected:
	
	virtual void BeginPlay() override;
#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	
	/* IRPGInteract_Interface */
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
	URPGInventory_Component* ContainerInventoryComp;

	/* Items to fill this container's inventory with. Use this to set what items should be in this container. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FContainerItem> ItemToAdd;

private:

	void FillContainerInventory();
};
