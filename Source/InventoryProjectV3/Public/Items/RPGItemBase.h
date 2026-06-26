// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/RPGInventoryConfig.h"
#include "Interfaces/RPGInteractInterface.h"
#include "RPGItemBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class USceneComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPGItemBase : public AActor, public IRPGInteractInterface
{
	GENERATED_BODY()
	
public:	

	ARPGItemBase();

	/* IRPGInteractInterface */
	virtual void InteractNative(AActor* Interactor) override;
	virtual FText GetNameNative() const override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddToInventory(AActor* Interactor, int32 QuantityOfItemsToAdd);

	/* Blueprint overridable function that determines how many items is given upon picking up */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	int32 GetQuantity() const;

	// TODO: Think how this could be overriden in child C++ classes
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
