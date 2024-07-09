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

	/** TODO: Remove BlueprintReadWrite when "magical" BTTask will be moved to code (or when an alternative is created) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character")
	float BreastsScaleMultiplier;

};
