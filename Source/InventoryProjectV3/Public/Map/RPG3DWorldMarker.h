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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	UWidgetComponent* WidgetComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "3D World Marker")
	TSubclassOf<URPG3DWorldMarkerWidget> WorldMarkerWidgetClass;

protected:
	
	virtual void BeginPlay() override;
	
	void UpdateDistance();
	void UpdateOwner();
	void Initialize();
	
};
