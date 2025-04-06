// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGQuestMarkerLocation.generated.h"

class USphereComponent;
class URPGPointOfInterestComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPGQuestMarkerLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPGQuestMarkerLocation();

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Marker")
	USphereComponent* SphereCollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Marker")
	URPGPointOfInterestComponent* PointOfInterestComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Marker")
	FText Name;
};
