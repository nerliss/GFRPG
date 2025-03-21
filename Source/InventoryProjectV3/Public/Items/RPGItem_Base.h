// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/RPGInventoryConfig.h"
#include "Interfaces/RPGInteract_Interface.h"
#include "RPGItem_Base.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class USceneComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPGItem_Base : public AActor, public IRPGInteract_Interface
{
	GENERATED_BODY()
	
public:	

	ARPGItem_Base();

	/* IRPGInteract_Interface */
	virtual void InteractNative(AActor* Interactor) override;
	virtual FText GetNameNative() const override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddToInventory(AActor* Interactor, int32 QuantityOfItemsToAdd);

	/* Blueprint overridable function that determines how many items is given upon picking up */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	int32 GetQuantity() const;

	// MyTODO: Think how this could be overriden in child C++ classes
	/* Blueprint overridable function that determines what happens when we use the item */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	bool UseItem() const;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* ItemSkeletalMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemStaticMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* TraceSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	FInventoryItem Item;

};
