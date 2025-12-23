// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGAbilityComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URPGAbilityComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
