// Oleksandr Tkachov 2021-2026

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
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:

	virtual void BeginPlay() override;
	
	void TakeScreenshot();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	float MapSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	FVector CameraLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	FVector BorderScale;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Map")
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UStaticMeshComponent* TopLeftMarker;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UStaticMeshComponent* TopRightMarker;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UStaticMeshComponent* BottomLeftMarker;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UStaticMeshComponent* BottomRightMarker;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Map")
	USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Map")
	UInstancedStaticMeshComponent* InstancedSM;
};
