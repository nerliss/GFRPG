// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGAbilityDefinitionData.h"

#include "UObject/ObjectSaveContext.h"

URPGAbilityDefinitionData::URPGAbilityDefinitionData()
{
	AbilityClass = nullptr;
	Icon = nullptr;
	Name = FText();
	Description = FText();
	Cooldown = 3.f;
	Cost = 3.f;
	CastRange = 1200.f;
	bDoGroundTrace = false;
	GroundTraceDistance = 400.f;
	ParticleSystem = nullptr;
	ActivationMode = EAbilityActivationMode::Instant;
	TargetingFlow = EAbilityTargetingFlow::Instant;
	ChannelParams = FChannelParams();
	CastParams = FCastParams();
	EffectsToApplyOnTarget = TArray<URPGEffectDefinitionData*>();
	EffectsToApplyOnSelf = TArray<URPGEffectDefinitionData*>();
}

URPGSummonAbilityDefinitionData::URPGSummonAbilityDefinitionData()
{
	ActorToSpawn = nullptr;
	PreviewMesh = nullptr;
	ActivationMode = EAbilityActivationMode::Cast;
	TargetingFlow = EAbilityTargetingFlow::PreviewConfirm;
}

void URPGSummonAbilityDefinitionData::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);
	
	if (!ActorToSpawn)
	{
		PreviewMesh = nullptr;
		return;
	}
	
	const AActor* ActorCDO = ActorToSpawn.GetDefaultObject();
	if (!ActorCDO)
	{
		PreviewMesh = nullptr;
		return;
	}
	
	const USkeletalMeshComponent* SkeletalMeshComp = ActorCDO->GetComponentByClass<USkeletalMeshComponent>();
	if (!SkeletalMeshComp)
	{
		PreviewMesh = nullptr;
		return;
	}
	
	PreviewAnimClass = SkeletalMeshComp->AnimClass;
	PreviewMesh = SkeletalMeshComp->GetSkeletalMeshAsset();
	SpawnScale = SkeletalMeshComp->GetRelativeScale3D();
}

URPGAOEAbilityDefinitionData::URPGAOEAbilityDefinitionData()
{
	TargetingFlow = EAbilityTargetingFlow::PreviewConfirm;
}

URPGProjectileAbilityDefinitionData::URPGProjectileAbilityDefinitionData()
{
	ProjectileToSpawn = nullptr;
}

URPGModifyStatAbilityDefinitionData::URPGModifyStatAbilityDefinitionData()
{
	StatToModify = EAbilityStatToModifyType::Health;
	StatModificationType = EStatModificationType::Percentage;
	StatModificationOperation = EStatModificationOperation::Add;
	AmountToModify = 1.f;
	PercentageToModify = 10.f;
}
