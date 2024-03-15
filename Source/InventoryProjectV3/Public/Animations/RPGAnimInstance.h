// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGAnimInstance.generated.h"

class ARPGPlayerCharacter;

/**
 * Base anim instance class for all ABPs.
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	URPGAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	// MyTODO: Add IKFeetAlpha that is calculated in code and used in ABP instead of this
	/* Should we update IK feet? */ 
	UFUNCTION(BlueprintPure, Category = "IK Feet")
	bool ShouldUpdateIKFeet() const;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	ARPGPlayerCharacter* Character;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	bool bDead;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	bool bMounted;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	bool bFalling;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	float MovementSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "IK Feet")
	bool bIKFeetEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IK Feet")
	FName IKRightFootSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "IK Feet")
	FName IKLeftFootSocketName;

	UPROPERTY(BlueprintReadWrite, Category = "IK Feet")
	float IKHipOffset;

	UPROPERTY(BlueprintReadWrite, Category = "IK Feet")
	FVector IKLeftFootEffector;

	UPROPERTY(BlueprintReadWrite, Category = "IK Feet")
	FVector IKRightFootEffector;

private:

	/**
	 * Trace the ground from the specified foot socket to calculate the foot offset.
	 * @param SocketName Socket name to start the trace from.
	 * @param TraceDistance Used to detect how deep under the Socket should the trace go. Usually passed from character's capsule half-size.
	 * @param OutTraceHitLocation Return value of found trace hit location.
	 * @param OutFootTraceOffset Return value of specified foot offset.
	 */
	void CalculateIKFootTrace(const FName SocketName, const float TraceDistance, FVector& OutTraceHitLocation, float& OutFootTraceOffset);
	void UpdateIKFeet();

	float IKRightFootOffset;
	float IKLeftFootOffset;

};
