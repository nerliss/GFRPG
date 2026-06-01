// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "RPGAbilityModifyStat.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityModifyStat : public URPGAbilityBase
{
	GENERATED_BODY()
	
public:
	
	virtual void InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor, URPGAbilityDefinitionData* InAbilityDefinitionData) override;
	virtual bool UseAbility(FRPGTargetData TargetData) override;
	virtual void OnCastComplete(FRPGTargetData TargetData) override;
	
	// TODO: Think over this idea for the whole system since when OnCastComplete calls UseAbility and said ability has a cd longer than cast time the ability will not pass parent check due to being on cooldown already
	bool bFromCast;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URPGModifyStatAbilityDefinitionData* ModifyStatAbilityDefinitionData;
};
