// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Animations/RPGAnimInstance.h"
#include "FemaleAnimInstance.generated.h"

/**
 * Base anim instance for female characters. 
 */
UCLASS()
class INVENTORYPROJECTV3_API UFemaleAnimInstance : public URPGAnimInstance
{
	GENERATED_BODY()

public:

	UFemaleAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	float BreastsScaleMultiplier;

};
