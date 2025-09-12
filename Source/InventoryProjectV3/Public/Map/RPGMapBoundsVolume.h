// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGMapBoundsVolume.generated.h"

UCLASS()
class INVENTORYPROJECTV3_API ARPGMapBoundsVolume : public AActor
{
	GENERATED_BODY()
	
public:
	
	ARPGMapBoundsVolume();

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
