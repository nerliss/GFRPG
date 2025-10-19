// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG3DWorldMarker.generated.h"

class URPG3DWorldMarkerWidget;
class UWidgetComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPG3DWorldMarker : public AActor
{
	GENERATED_BODY()
	
public:
	
	ARPG3DWorldMarker();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	TSubclassOf<URPG3DWorldMarkerWidget> WorldMarkerWidgetClass;
	
	FText WaypointName;

	bool bQuestWaypoint;

	float MaxShowDistance;
	
protected:
	
	virtual void BeginPlay() override;
	
	void UpdateDistance();
	void UpdateOwner();
	void Initialize();

	float PlayerDistance;

	FTimerHandle UpdateDistanceHandle;

	UPROPERTY()
	URPG3DWorldMarkerWidget* UIWorldMarker;
	
};
