// Oleksandr Tkachov 2021-2026


#include "Abilities/Archetypes/RPGAbilityDisplace.h"

void URPGAbilityDisplace::InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor,
	URPGAbilityDefinitionData* InAbilityDefinitionData)
{
	Super::InitAbility(InAbilityComponent, InOwnerActor, InAbilityDefinitionData);
	
	DisplaceAbilityDefinitionData = Cast<URPGDisplaceAbilityDefinitionData>(InAbilityDefinitionData);
}

bool URPGAbilityDisplace::UseAbility(FRPGTargetData TargetData)
{
	return Super::UseAbility(TargetData);
}
