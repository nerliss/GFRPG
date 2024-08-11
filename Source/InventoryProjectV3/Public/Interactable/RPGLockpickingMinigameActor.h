// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGLockpickingMinigameActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USceneCaptureComponent2D;
class URPGLockpickingMinigameWidget;

UCLASS()
class INVENTORYPROJECTV3_API ARPGLockpickingMinigameActor : public AActor
{
	GENERATED_BODY()
	
public:

	ARPGLockpickingMinigameActor();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LockMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* LeftPickScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* RightPickScene;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LeftPickMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RightPickMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(EditDefaultsOnly, Category = "LockpickingMinigame")
	TSubclassOf<URPGLockpickingMinigameWidget> LockpickingMinigameWidgetClass;

private:


};
