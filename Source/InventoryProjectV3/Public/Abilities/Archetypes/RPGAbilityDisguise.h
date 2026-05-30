// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "RPGAbilityDisguise.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityDisguise : public URPGAbilityBase
{
	GENERATED_BODY()
	
public:
	
	virtual void InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor, URPGAbilityDefinitionData* InAbilityDefinitionData) override;
	virtual bool UseAbility(FRPGTargetData TargetData) override;
	virtual URPGDisguiseAbilityDefinitionData* GetAbilityDefinition() const override { return DisguiseAbilityDefinitionData; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URPGDisguiseAbilityDefinitionData* DisguiseAbilityDefinitionData;
};
