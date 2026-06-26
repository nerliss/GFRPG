// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/RPGInteractInterface.h"
#include "RPG3DWorldMarker.generated.h"

class URPGPointOfInterestComponent;
class URPG3DWorldMarkerWidget;
class UWidgetComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPG3DWorldMarker : public AActor, public IRPGInteractInterface
{
	GENERATED_BODY()
	
public:
	
	ARPG3DWorldMarker();

	/* RPGInteractable_Interface */
	virtual void InteractNative(AActor* Interactor) override;
	virtual FText GetNameNative() const override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	UWidgetComponent* WidgetComp;

	/* Arrow-like mesh component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	UStaticMeshComponent* MeshComponent;

	/* A pillar of light-like that is visible from far away mesh component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	UStaticMeshComponent* HighlightMeshComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	URPGPointOfInterestComponent* PointOfInterestComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	TSubclassOf<URPG3DWorldMarkerWidget> WorldMarkerWidgetClass;
	
	FText WaypointName;

	bool bQuestWaypoint;

	float MaxShowDistance;
	
protected:
	
	virtual void BeginPlay() override;
	
	void UpdateDistance();
	void Initialize();

	float PlayerDistance;

	FTimerHandle UpdateDistanceHandle;

	UPROPERTY()
	URPG3DWorldMarkerWidget* UIWorldMarker;
	
};
