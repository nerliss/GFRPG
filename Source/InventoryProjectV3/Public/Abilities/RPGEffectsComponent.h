// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGEffectsComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URPGEffectsComponent();

protected:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
