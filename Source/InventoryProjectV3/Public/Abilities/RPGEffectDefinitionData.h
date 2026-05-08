// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGEffectDefinitionData.generated.h"

UENUM()
enum class EEffectsDurationPolicy
{
	Instant,
	Duration,
	Infinite,
	MAX UMETA(Hidden)
};

UENUM()
enum class EEffectsStackPolicy
{
	NoStack,
	Stack,
	RefreshDuration,
	StackAndRefreshDuration,
	MAX UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGEffectDefinitionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
		
	URPGEffectDefinitionData();
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void PostLoad() override;
	
	UFUNCTION(BlueprintPure)
	FPrimaryAssetId GetEffectId() const { return GetPrimaryAssetId(); }
	
	UPROPERTY(VisibleDefaultsOnly, Transient, BlueprintReadOnly, Category = "Ability System")
	FPrimaryAssetId CachedId;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	EEffectsDurationPolicy DurationPolicy;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	float Duration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	float Period;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	bool bExecuteOnApply;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	EEffectsStackPolicy StackPolicy;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System", meta = (EditCondition = "StackPolicy != EEffectsStackPolicy::NoStack"))
	int32 MaxStacks;
	
};
