// Oleksandr Tkachov 2021-2026


#include "Abilities/Archetypes/RPGAbilitySummon.h"

void URPGAbilitySummon::InitAbility(URPGAbilityComponent* FromAbilityComponent, AActor* FromOwnerActor,
	URPGAbilityDefinitionData* FromAbilityDefinitionData)
{
	Super::InitAbility(FromAbilityComponent, FromOwnerActor, FromAbilityDefinitionData);
	
	SummonAbilityDefinitionData = Cast<URPGSummonAbilityDefinitionData>(FromAbilityDefinitionData);
}

bool URPGAbilitySummon::UseAbility(FRPGTargetData TargetData)
{
	return Super::UseAbility(TargetData);
}

void URPGAbilitySummon::OnCastComplete(FRPGTargetData TargetData)
{
	Super::OnCastComplete(TargetData);
}
