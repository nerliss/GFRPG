// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGAbilityPickup.generated.h"

class URPGAbilityDefinitionData;
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	URPGAbilityDefinitionData* AbilityDefinition;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	UMaterialInterface* AbilityIconMeshMaterial;
	
protected:
	
	virtual void BeginPlay() override;
#if WITH_EDITORONLY_DATA
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	
	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
private:
	
	void CreateAbilityIconMeshDynamicMaterial();
	
};
