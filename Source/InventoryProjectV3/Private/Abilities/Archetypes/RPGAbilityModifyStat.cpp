// Oleksandr Tkachov 2021-2026


#include "Abilities/Archetypes/RPGAbilityModifyStat.h"

#include "Components/RPGHealth_Component.h"
#include "Components/RPGXP_Component.h"

void URPGAbilityModifyStat::InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor,
                                        URPGAbilityDefinitionData* InAbilityDefinitionData)
{
	Super::InitAbility(InAbilityComponent, InOwnerActor, InAbilityDefinitionData);
	
	ModifyStatAbilityDefinitionData = Cast<URPGModifyStatAbilityDefinitionData>(InAbilityDefinitionData);
}

bool URPGAbilityModifyStat::UseAbility(FRPGTargetData TargetData)
{
	if (!Super::UseAbility(TargetData) && !bFromCast)
	{
		return false;
	}
	
	bFromCast = false;
	
	if (!ModifyStatAbilityDefinitionData)
	{
		return false;
	}
	
	if (!OwnerActor)
	{
		return false;
	}

	switch (ModifyStatAbilityDefinitionData->StatToModify)
	{
	case EAbilityStatToModifyType::Health:
		{
			URPGHealth_Component* HealthComp = Cast<URPGHealth_Component>(OwnerActor->GetComponentByClass(URPGHealth_Component::StaticClass()));
			if (HealthComp)
			{
				const float AmountToModify = ModifyStatAbilityDefinitionData->AmountToModify;
				const float PercentageToModify = HealthComp->GetMaxHealth() * (ModifyStatAbilityDefinitionData->PercentageToModify / 100.f);
				const float ModificationTypeValue = ModifyStatAbilityDefinitionData->StatModificationType == EStatModificationType::Flat ? AmountToModify : PercentageToModify;
				const bool bAdd = ModifyStatAbilityDefinitionData->StatModificationOperation == EStatModificationOperation::Add;
				
				// ModifyCurrentHealth incorporates ApplyDamage function so we can use it to damage an actor
				HealthComp->ModifyCurrentHealth(bAdd ? ModificationTypeValue : -ModificationTypeValue);
				return true;
			}
			return false;
		}
		
	case EAbilityStatToModifyType::XP:
		{
			URPGXP_Component* XPComp = Cast<URPGXP_Component>(OwnerActor->GetComponentByClass(URPGXP_Component::StaticClass()));
			if (XPComp)
			{
				const float AmountToModify = ModifyStatAbilityDefinitionData->AmountToModify;
				const float PercentageToModify = XPComp->GetCurrentMaxXP() * (ModifyStatAbilityDefinitionData->PercentageToModify / 100.f);
			
				// We shouldn't be able to subtract experience points, so only Add operation is supported 
				XPComp->AddXP(ModifyStatAbilityDefinitionData->StatModificationType == EStatModificationType::Flat ? AmountToModify : PercentageToModify);
				return true;
			}
			return false;
		}
		// TODO: Support other stats when they are implemented
	case EAbilityStatToModifyType::Agility:
	case EAbilityStatToModifyType::Damage:
	case EAbilityStatToModifyType::Intellect:
	case EAbilityStatToModifyType::Resource:
	case EAbilityStatToModifyType::Reputation:
	case EAbilityStatToModifyType::Strength:
		return false;
		
	default:
		return false;
	}
}

void URPGAbilityModifyStat::OnCastComplete(FRPGTargetData TargetData)
{
	bFromCast = true;
	UseAbility(TargetData);
}
