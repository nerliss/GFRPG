// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGPointOfInterestComponent.generated.h"

UENUM(BlueprintType)
enum class EPOIMobility : uint8
{
	Static,
	Dynamic,
	None
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYPROJECTV3_API URPGPointOfInterestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URPGPointOfInterestComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void SpawnPOIOnMap();

	FSlateBrush GetIcon() const { return Icon; }

	// TODO: Remove
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point of interest")
	bool bQuestObjective;

	// TODO: Remove
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point of interest")
	EPOIMobility Mobility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point of interest")
	FSlateBrush Icon;

	/* Should icon be rotated with actor? Useful for Player icon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point of interest")
	bool bRotateWithActor;

	/* Should icon always be visible on a map? Currently, affects only Minimap since Map Screen's icons are always visible by default */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point of interest")
	bool bAlwaysShow;

private:

	bool bSpawned;

};
