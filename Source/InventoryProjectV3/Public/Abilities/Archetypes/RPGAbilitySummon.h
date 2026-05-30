// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "RPGAbilitySummon.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilitySummon : public URPGAbilityBase
{
	GENERATED_BODY()
	
public:
	
	virtual void InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor, URPGAbilityDefinitionData* InAbilityDefinitionData) override;
	virtual bool UseAbility(FRPGTargetData TargetData) override;
	virtual void OnCastComplete(FRPGTargetData TargetData) override;
	virtual void OnCastInterrupted(FRPGTargetData TargetData, EAbilityInterruptReason Reason) override;
	virtual void OnToggleStarted(FRPGTargetData TargetData) override;
	virtual void OnToggleEnded(FRPGTargetData TargetData) override;
	virtual void OnTargetingPreviewStarted(FRPGTargetData TargetData) override;
	virtual void OnTargetingPreviewUpdated(FRPGTargetData TargetData) override;
	virtual void OnTargetingPreviewCanceled(FRPGTargetData TargetData) override;
	virtual void OnTargetingPreviewConfirmed(FRPGTargetData TargetData) override;
	virtual URPGSummonAbilityDefinitionData* GetAbilityDefinition() const override { return SummonAbilityDefinitionData; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URPGSummonAbilityDefinitionData* SummonAbilityDefinitionData;
	
	UPROPERTY(BlueprintReadOnly)
	AActor* ToggleSpawnedActor;
};
