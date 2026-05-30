// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "RPGAbilityAOE.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityAOE : public URPGAbilityBase
{
	GENERATED_BODY()
	
public:
	
	virtual void InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor, URPGAbilityDefinitionData* InAbilityDefinitionData) override;
	virtual bool UseAbility(FRPGTargetData TargetData) override;
	virtual URPGAOEAbilityDefinitionData* GetAbilityDefinition() const override { return AOEAbilityDefinitionData; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URPGAOEAbilityDefinitionData* AOEAbilityDefinitionData;
};
