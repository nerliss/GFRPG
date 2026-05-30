// Oleksandr Tkachov 2021-2026


#include "Abilities/Archetypes/RPGAbilityProjectile.h"

void URPGAbilityProjectile::InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor,
	URPGAbilityDefinitionData* InAbilityDefinitionData)
{
	Super::InitAbility(InAbilityComponent, InOwnerActor, InAbilityDefinitionData);
	
	ProjectileAbilityDefinitionData = Cast<URPGProjectileAbilityDefinitionData>(InAbilityDefinitionData);
}

bool URPGAbilityProjectile::UseAbility(FRPGTargetData TargetData)
{
	return Super::UseAbility(TargetData);
}
