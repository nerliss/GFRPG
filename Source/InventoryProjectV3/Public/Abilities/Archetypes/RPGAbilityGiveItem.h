// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "RPGAbilityGiveItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityGiveItem : public URPGAbilityBase
{
	GENERATED_BODY()
	
public:
	
	virtual void InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor, URPGAbilityDefinitionData* InAbilityDefinitionData) override;
	virtual bool UseAbility(FRPGTargetData TargetData) override;
	virtual URPGGiveItemAbilityDefinitionData* GetAbilityDefinition() const override { return GiveItemAbilityDefinitionData; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URPGGiveItemAbilityDefinitionData* GiveItemAbilityDefinitionData;
};
