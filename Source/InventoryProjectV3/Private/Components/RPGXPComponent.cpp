// Oleksandr Tkachov 2021-2026


#include "Components/RPGXPComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

URPGXPComponent::URPGXPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CapLevel = 30;
	SkillPoints = 0;
}

void URPGXPComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevel = 1;

	CurrentXP = 0.f;
	CurrentMaxXP = 100.f;

	CalculateMaxXP();

	//OnLevelGained.AddDynamic(this, &URPGXPComponent::CalculateXPReward);
}

void URPGXPComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentXP = UKismetMathLibrary::FCeil(UKismetMathLibrary::FInterpTo(CurrentXP, BufferXP, DeltaTime, 3.f));

	CalculateXPPercentage();
	CalculateXPBufferPercentage();

	if (CurrentXP >= CurrentMaxXP)
	{
		LevelUp();
	}
}

void URPGXPComponent::CalculateXPPercentage()
{
	XPCurrentPercentage = CurrentXP / CurrentMaxXP;
}

void URPGXPComponent::CalculateXPBufferPercentage()
{
	BufferXPCurrentPercentage = BufferXP / CurrentMaxXP;
}

void URPGXPComponent::CalculateMaxXP()
{
	// Temp equation
	// I kinda like the way it works, because quests and other activities will always grant a fraction
	// of Current_Max XP, thus making this calculation at some point useless (it will always take you almost the same
	// time to gain a level no matter of the level - whether you are lvl 1 or lvl 20)
	// Perhaps in future I will replace this with datatable
	CurrentMaxXP = UKismetMathLibrary::FCeil(CurrentMaxXP * 1.7f);
}

void URPGXPComponent::AddXP(float AddedXP)
{
	BufferXP = AddedXP + BufferXP;
}

void URPGXPComponent::LevelUp()
{
	if (CurrentLevel >= CapLevel)
	{
		CurrentXP = 0.f;
		BufferXP = 0.f;
	}
	else
	{
		CurrentLevel++;

		float XP_Remaining = BufferXP - CurrentMaxXP; 

		CalculateMaxXP();

		CurrentXP = 0.f;
		BufferXP = 0.f;

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
		return CurrentMaxXP * 0.15 * Multiplier; // Quests award 15% of required XP
	}
	else
	{
		return CurrentMaxXP * 0.033 * Multiplier; // Kills (and maybe something else) award 3.3% of required XP
	}
}
