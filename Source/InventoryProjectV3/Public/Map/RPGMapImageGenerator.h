// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGMapImageGenerator.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USplineComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPGMapImageGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	
	ARPGMapImageGenerator();

protected:

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	void TakeScreenshot();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	float MapSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	FVector CameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	FVector BorderScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UStaticMeshComponent* TopLeftMarker;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UStaticMeshComponent* TopRightMarker;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UStaticMeshComponent* BottomLeftMarker;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UStaticMeshComponent* BottomRightMarker;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	USplineComponent* Spline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UInstancedStaticMeshComponent* InstancedSM;
};
