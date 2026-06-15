// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGEffectsComponent.generated.h"

class URPGEffectDefinitionData;

USTRUCT(BlueprintType)
struct FRPGActiveEffect
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URPGEffectDefinitionData> Definition;

	UPROPERTY(BlueprintReadOnly)
	float RemainingDuration;

	UPROPERTY(BlueprintReadOnly)
	float PeriodAccumulator;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentStacks;

	FTimerHandle DurationHandle;

	bool IsValid() const { return Definition != nullptr; }
	
	FRPGActiveEffect()
	{
		Definition = nullptr;
		RemainingDuration = 0.f;
		PeriodAccumulator = 0.f;
		CurrentStacks = 1;
	}
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectApplied, URPGEffectDefinitionData*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectRemoved, URPGEffectDefinitionData*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEffectStackUpdated, URPGEffectDefinitionData*, Effect, int32, NewStacks);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	URPGEffectsComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ApplyEffect(URPGEffectDefinitionData* EffectDefinition, AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void RemoveEffect(URPGEffectDefinitionData* EffectDefinition);
	
	UFUNCTION(BlueprintPure, Category = "Effects")
	bool HasEffect(URPGEffectDefinitionData* EffectDefinition) const;
	
	UFUNCTION(BlueprintPure, Category = "Effects")
	float GetEffectRemainingDuration(URPGEffectDefinitionData* EffectDefinition) const;
	
	UFUNCTION(BlueprintPure, Category = "Effects")
	int32 GetEffectStacks(URPGEffectDefinitionData* EffectDefinition) const;

	UPROPERTY(BlueprintAssignable)
	FOnEffectApplied OnEffectApplied;
	
	UPROPERTY(BlueprintAssignable)
	FOnEffectRemoved OnEffectRemoved;
	
	UPROPERTY(BlueprintAssignable)
	FOnEffectStackUpdated OnEffectStackUpdated;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TArray<FRPGActiveEffect> ActiveEffects;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
private:
	
	void ExecuteEffect(URPGEffectDefinitionData* EffectDefinition, AActor* Instigator);
	void HandleStacking(FRPGActiveEffect& ExistingEffect, URPGEffectDefinitionData* EffectDefinition);
	
	UFUNCTION()
	void OnEffectDurationExpired(URPGEffectDefinitionData* EffectDefinition);
	
};
