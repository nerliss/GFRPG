// Oleksandr Tkachov 2022-2024

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

protected:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point of interest")
	EPOIMobility Mobility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Point of interest")
	FSlateBrush Icon;

private:

	bool bSpawned;

public:

	UFUNCTION(BlueprintCallable)
	void SpawnPOIOnMap();

	FSlateBrush GetIcon() const { return Icon; }
};
