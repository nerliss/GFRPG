// Oleksandr Tkachov 2021-2026


#include "Abilities/Archetypes/RPGAbilityDisguise.h"

void URPGAbilityDisguise::InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor,
                                      URPGAbilityDefinitionData* InAbilityDefinitionData)
{
	Super::InitAbility(InAbilityComponent, InOwnerActor, InAbilityDefinitionData);
	
	DisguiseAbilityDefinitionData = Cast<URPGDisguiseAbilityDefinitionData>(InAbilityDefinitionData);
}

bool URPGAbilityDisguise::UseAbility(FRPGTargetData TargetData)
{
	return Super::UseAbility(TargetData);
}
