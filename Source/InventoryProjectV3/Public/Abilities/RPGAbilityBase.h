// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGAbilityBase.generated.h"

UCLASS()
class INVENTORYPROJECTV3_API ARPGAbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPGAbilityBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
