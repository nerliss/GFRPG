// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGEffectDefinitionData.h"

URPGEffectDefinitionData::URPGEffectDefinitionData()
{
	Name = FText();
	Description = FText();
	DurationPolicy = EEffectsDurationPolicy::Duration;
	Duration = 3.f;
	Period = 0.5f;
	bExecuteOnApply = true;
	StackPolicy = EEffectsStackPolicy::RefreshDuration;
	MaxStacks = 2;
}

FPrimaryAssetId URPGEffectDefinitionData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Effect"), GetFName());
}

void URPGEffectDefinitionData::PostLoad()
{
	Super::PostLoad();
	CachedId = GetPrimaryAssetId();
}
