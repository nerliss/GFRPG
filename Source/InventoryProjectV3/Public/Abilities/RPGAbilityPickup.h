// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGAbilityPickup.generated.h"

class USphereComponent;
class UInterpToMovementComponent;
class UStaticMeshComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPGAbilityPickup : public AActor
{
	GENERATED_BODY()
	
public:
	
	ARPGAbilityPickup();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UInterpToMovementComponent* InterpToMovementComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* AbilityIconMeshComponent;
	
protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
