// Oleksandr Tkachov 2021-2026


#include "Components/RPGReputationComponent.h"
#include "Utility/Utility.h"

URPGReputationComponent::URPGReputationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	Reputations.SetNum(5);
	Reputations[0] = { Faction_Orcs, 0.f};
	Reputations[1] = { Faction_Humans, 0.f};
	Reputations[2] = { Faction_Dwarfs, 0.f};
	Reputations[3] = { Faction_Elves, 0.f};
	Reputations[4] = { Faction_Demons, 0.f};
}

void URPGReputationComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void URPGReputationComponent::GetReputation(const TEnumAsByte<EFactions> FactionToGet, float& OutAmount,
	int32& OutIndex, TEnumAsByte<EFactions>& OutName)
{
	for (int32 i = 0; i < Reputations.Num(); ++i)
	{
		if (Reputations[i].FactionName == FactionToGet)
		{
			OutIndex = i;
			OutAmount = Reputations[i].CurrentReputation;
			OutName = Reputations[i].FactionName;
			break;
		}
	}
}

void URPGReputationComponent::AddReputation(TEnumAsByte<EFactions> Name, float Amount)
{
	float RepCurrentAmount;
	TEnumAsByte<EFactions> RepName;
	int32 RepIndex;

	GetReputation(Name, RepCurrentAmount, RepIndex, RepName);

	const float NewAmount = RepCurrentAmount + Amount;

	Reputations[RepIndex] = { RepName, NewAmount };
}
