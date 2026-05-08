// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGStatsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URPGStatsComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 Strength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 Agility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 Intellect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 Stamina; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float CritChance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float Speed;

	// TODO: Add more stats, implement their usage
	// TODO: Merge XPComponent (?) and HealthComponent here 
	
protected:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
