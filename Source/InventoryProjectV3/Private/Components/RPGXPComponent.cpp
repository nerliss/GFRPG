// Oleksandr Tkachov 2021-2026


#include "Components/RPGXPComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

URPGXPComponent::URPGXPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Level_Cap = 30;
	SkillPoints = 0;
}

void URPGXPComponent::BeginPlay()
{
	Super::BeginPlay();

	Level_Current = 1;

	XP_Current = 0.f;
	XP_Current_Max = 100.f;

	Calculate_MaxXP();

	//OnLevelGained.AddDynamic(this, &URPGXPComponent::CalculateXPReward);
}

void URPGXPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	XP_Current = UKismetMathLibrary::FCeil(UKismetMathLibrary::FInterpTo(XP_Current, XP_Buffer, DeltaTime, 3.f));

	Calculate_Percentage_XP();
	Calculate_Percentage_Buffer();

	if (XP_Current >= XP_Current_Max)
	{
		LevelUp();
	}
}

void URPGXPComponent::Calculate_Percentage_XP()
{
	CurrentPercentage_XP = XP_Current / XP_Current_Max;
}

void URPGXPComponent::Calculate_Percentage_Buffer()
{
	CurrentPercentage_Buffer = XP_Buffer / XP_Current_Max;
}

void URPGXPComponent::Calculate_MaxXP()
{
	// Temp equation
	// I kinda like the way it works, because quests and other activities will always grant a fraction
	// of Current_Max XP, thus making this calculation at some point useless (it will always take you almost the same
	// time to gain a level no matter of the level - whether you are lvl 1 or lvl 20)
	// Perhaps in future I will replace this with datatable
	XP_Current_Max = UKismetMathLibrary::FCeil(XP_Current_Max * 1.7f);
}

void URPGXPComponent::AddXP(float AddedXP)
{
	XP_Buffer = AddedXP + XP_Buffer;
}

void URPGXPComponent::LevelUp()
{
	if (Level_Current >= Level_Cap)
	{
		XP_Current = 0.f;
		XP_Buffer = 0.f;
	}
	else
	{
		Level_Current++;

		float XP_Remaining = XP_Buffer - XP_Current_Max; 

		Calculate_MaxXP();

		XP_Current = 0.f;
		XP_Buffer = 0.f;

		SkillPoints++;

		AddXP(XP_Remaining);

		OnLevelGained.Broadcast();

		if (LevelUpSound)
		{
			UGameplayStatics::SpawnSound2D(this, LevelUpSound, 0.7f);
		}

		if (LevelUpParticle)
		{
			UGameplayStatics::SpawnEmitterAttached(LevelUpParticle, GetOwner()->GetRootComponent());
		}
	}
}

float URPGXPComponent::CalculateXPReward(bool bQuestReward, float Multiplier)
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
