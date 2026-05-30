// Oleksandr Tkachov 2021-2026


#include "Abilities/Archetypes/RPGAbilityAOE.h"

void URPGAbilityAOE::InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor,
                                 URPGAbilityDefinitionData* InAbilityDefinitionData)
{
	Super::InitAbility(InAbilityComponent, InOwnerActor, InAbilityDefinitionData);
	
	AOEAbilityDefinitionData = Cast<URPGAOEAbilityDefinitionData>(InAbilityDefinitionData);
}

bool URPGAbilityAOE::UseAbility(FRPGTargetData TargetData)
{
	return Super::UseAbility(TargetData);
}
