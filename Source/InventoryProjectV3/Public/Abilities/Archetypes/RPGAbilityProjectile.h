// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "RPGAbilityProjectile.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityProjectile : public URPGAbilityBase
{
	GENERATED_BODY()
	
public:
	
	virtual void InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor, URPGAbilityDefinitionData* InAbilityDefinitionData) override;
	virtual bool UseAbility(FRPGTargetData TargetData) override;
	virtual URPGProjectileAbilityDefinitionData* GetAbilityDefinition() const override { return ProjectileAbilityDefinitionData; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	URPGProjectileAbilityDefinitionData* ProjectileAbilityDefinitionData;
};
