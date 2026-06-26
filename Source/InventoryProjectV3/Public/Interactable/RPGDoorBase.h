// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/RPGInteractInterface.h"
#include "RPGDoorBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class ARPGLockpickingMinigameActor;

UCLASS()
class INVENTORYPROJECTV3_API ARPGDoorBase : public AActor, public IRPGInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGDoorBase();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* IRPGInteractInterface */
	virtual void InteractNative(AActor* Interactor) override;
	virtual FText GetNameNative() const override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FrameMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* TraceSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Door")
	FText DoorName;

	/* Should we play lockpicking minigame first? Editable per instance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	bool bClosed;

	UPROPERTY(EditDefaultsOnly, Category = "Door")
	TSubclassOf<ARPGLockpickingMinigameActor> LockpickingMinigameClass;

private:

	void OpenDoor();
	void CloseDoor();

	
};
