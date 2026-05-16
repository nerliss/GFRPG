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
	
	virtual void InitAbility(URPGAbilityComponent* FromAbilityComponent, AActor* FromOwnerActor, URPGAbilityDefinitionData* FromAbilityDefinitionData) override;
	virtual bool UseAbility(FRPGTargetData& TargetData) override;
	virtual void OnCastComplete(FRPGTargetData& TargetData) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URPGModifyStatAbilityDefinitionData* ModifyStatAbilityDefinitionData;
};
