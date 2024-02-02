// Oleksandr Tkachov 2022-2024


#include "Components/RPGReputation_Component.h"
#include "Utility/Utility.h"


// Sets default values for this component's properties
URPGReputation_Component::URPGReputation_Component()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// Set number of elements (must be equal to amount of factions in EFactions)
	Reputations.SetNum(5);

	// Set default values for each element
	Reputations[0] = { Faction_Orcs, 0.f};
	Reputations[1] = { Faction_Humans, 0.f};
	Reputations[2] = { Faction_Dwarfs, 0.f};
	Reputations[3] = { Faction_Elves, 0.f};
	Reputations[4] = { Faction_Demons, 0.f};
}

// Called when the game starts
void URPGReputation_Component::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void URPGReputation_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGReputation_Component::GetReputation(const TEnumAsByte<EFactions> In_Name /*input value*/,
	float& Out_Amount, int32& Out_Index, TEnumAsByte<EFactions>& Out_Name /*output values*/)
{
	for (int32 i = 0; i < Reputations.Num(); ++i)
	{
		// Check if current reputation has the same name as desired
		if (Reputations[i].Faction_name == In_Name)
		{
			// Return its index
			Out_Index = i;

			// Return its amount of reputation
			Out_Amount = Reputations[i].Reputation_Amount;

			// Return its name
			Out_Name = Reputations[i].Faction_name;

			// Break the loop
			break;
		}
	}
}

void URPGReputation_Component::AddReputation(TEnumAsByte<EFactions> Name, float Amount)
{
	// Temp variables that hold return values from GetReputationAmount()
	float RepCurrentAmount;
	TEnumAsByte<EFactions> RepName;
	int32 RepIndex;

	// Get return values 
	GetReputation(Name, RepCurrentAmount, RepIndex, RepName);

	// Value to update in desired reputation
	float NewAmount = RepCurrentAmount + Amount;

	// Update element of the reputations array
	Reputations[RepIndex] = { RepName, NewAmount };
}
