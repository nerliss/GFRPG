// Oleksandr Tkachov 2022-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGProjectileBase.generated.h"

class UProjectileMovementComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPGProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ARPGProjectileBase();
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
};
