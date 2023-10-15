// Oleksandr Tkachov 2022-2023


#include "Components/RPGXP_Component.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
URPGXP_Component::URPGXP_Component()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Default level cap
	Level_Cap = 30;

	// Default SP
	SkillPoints = 0;
}

// Called when the game starts
void URPGXP_Component::BeginPlay()
{
	Super::BeginPlay();

	// Set up default variables for the component
	Level_Current = 1;

	XP_Current = 0.f;
	XP_Current_Max = 100.f;

	Calculate_MaxXP();

	//OnLevelGained.AddDynamic(this, &URPGXP_Component::CalculateXPReward);
}

// Called every frame
void URPGXP_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Interpolate from current xp value to new xp value (buffer) thus making an smooth animation
	XP_Current = UKismetMathLibrary::FCeil(UKismetMathLibrary::FInterpTo(XP_Current, XP_Buffer, DeltaTime, 3.f));

	// Calculate percentage for progress bars 
	Calculate_Percentage_XP();
	Calculate_Percentage_Buffer();

	// Level up
	if (XP_Current >= XP_Current_Max)
	{
		LevelUp();
	}
}

// For progress bar
void URPGXP_Component::Calculate_Percentage_XP()
{
	CurrentPercentage_XP = XP_Current / XP_Current_Max;
}

void URPGXP_Component::Calculate_Percentage_Buffer()
{
	CurrentPercentage_Buffer = XP_Buffer / XP_Current_Max;
}

void URPGXP_Component::Calculate_MaxXP()
{
	// Temp equation
	// I kinda like the way it works, because quests and other activities will always grant a fraction
	// of Current_Max XP, thus making this calculation at some point useless (it will always take you almost the same
	// time to gain a level no matter of the level - whether you are lvl 1 or lvl 20)
	// Perhaps in future I will replace this with datatable
	XP_Current_Max = UKismetMathLibrary::FCeil(XP_Current_Max * 1.7f);
}

void URPGXP_Component::AddXP(float AddedXP)
{
	XP_Buffer = AddedXP + XP_Buffer;
}

// If level cap is reached - zero bars, if not - preform a level up
void URPGXP_Component::LevelUp()
{
	if (Level_Current >= Level_Cap)
	{
		XP_Current = 0.f;
		XP_Buffer = 0.f;
	}
	else
	{
		Level_Current++;

		// How much experience points we've exceeded XP_Current_Max by
		float XP_Remaining = XP_Buffer - XP_Current_Max; 

		// Calculate MaxXP for next level
		Calculate_MaxXP();

		// Zero bars
		XP_Current = 0.f;
		XP_Buffer = 0.f;

		// Add 1 skill point
		SkillPoints++;

		// Add remaining xp to next level
		AddXP(XP_Remaining);

		// Call blueprint implementable event (called in player's bp to update XP rewards for quest for now)
		OnLevelGained.Broadcast();

		/* Cosmetics */
		// Play SFX
		if (LevelUpSound)
		{
			UGameplayStatics::SpawnSound2D(this, LevelUpSound, 0.7f);
		}

		// Play VFX
		if (LevelUpParticle)
		{
			UGameplayStatics::SpawnEmitterAttached(LevelUpParticle, GetOwner()->GetRootComponent());
		}
	}
}

float URPGXP_Component::CalculateXPReward(bool bQuestReward, float Multiplier)
{
	if (bQuestReward)
	{
		return XP_Current_Max * 0.15 * Multiplier; // Quests award 15% of required XP
	}
	else
	{
		return XP_Current_Max * 0.033 * Multiplier; // Kills (and maybe something else) award 3.3% of required XP
	}
}

float URPGXP_Component::GetXP_Current()
{
	return XP_Current;
}

float URPGXP_Component::GetXP_Current_Max()
{
	return XP_Current_Max;
}

float URPGXP_Component::GetCurrentPercentage_XP()
{
	return CurrentPercentage_XP;
}

float URPGXP_Component::GetCurrentPercentage_Buffer()
{
	return CurrentPercentage_Buffer;
}

int32 URPGXP_Component::GetSkillPoints()
{
	return SkillPoints;
}

int32 URPGXP_Component::GetLevelCurrent()
{
	return Level_Current;
}
