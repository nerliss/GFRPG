// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGAbilityBase.h"

#include "Abilities/RPGAbilityComponent.h"

URPGAbilityBase::URPGAbilityBase()
{
	AbilityDefinition = nullptr;
}

void URPGAbilityBase::InitAbility(URPGAbilityComponent* FromAbilityComponent, AActor* FromOwnerActor,
	URPGAbilityDefinitionData* FromAbilityDefinitionData)
{
	AbilityComponent = FromAbilityComponent;
	OwnerActor = FromOwnerActor;
	AbilityDefinition = FromAbilityDefinitionData;
}

bool URPGAbilityBase::CanUseAbility()
{
	return true;
}

bool URPGAbilityBase::TryUseAbility(FRPGTargetData TargetData)
{
	return true;
}
