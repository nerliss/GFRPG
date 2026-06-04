// Oleksandr Tkachov 2021-2026


#include "Abilities/Archetypes/RPGAbilityGiveItem.h"

void URPGAbilityGiveItem::InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor,
	URPGAbilityDefinitionData* InAbilityDefinitionData)
{
	Super::InitAbility(InAbilityComponent, InOwnerActor, InAbilityDefinitionData);
	
	GiveItemAbilityDefinitionData = Cast<URPGGiveItemAbilityDefinitionData>(InAbilityDefinitionData);
}

bool URPGAbilityGiveItem::UseAbility(FRPGTargetData TargetData)
{
	return Super::UseAbility(TargetData);
}
