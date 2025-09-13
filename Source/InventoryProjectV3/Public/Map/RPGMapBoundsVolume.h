// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGMapBoundsVolume.generated.h"

class UBoxComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPGMapBoundsVolume : public AActor
{
	GENERATED_BODY()
	
public:
	
	ARPGMapBoundsVolume();

	UFUNCTION(BlueprintCallable, Category="Map")
	void GetXYMinMax(FVector2D& OutMin, FVector2D& OutMax) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* Bounds;
	
};
