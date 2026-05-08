// Oleksandr Tkachov 2021-2026


#include "Characters/RPGCharacter.h"

#include "Abilities/RPGAbilityComponent.h"
#include "Components/RPGPointOfInterestComponent.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/RPGStatsComponent.h"

ARPGCharacter::ARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	QuestLogComponent = CreateDefaultSubobject<URPGQuestLogComponent>(TEXT("QuestLogComponent"));

	StatsComponent = CreateDefaultSubobject<URPGStatsComponent>(TEXT("StatsComponent"));

	CharacterName = FText::FromString("Default Character");

	PointOfInterestComponent = CreateDefaultSubobject<URPGPointOfInterestComponent>(TEXT("PointOfInterestComponent"));
	PointOfInterestComponent->Icon = FSlateBrush();

	AbilityComponent = CreateDefaultSubobject<URPGAbilityComponent>(TEXT("AbilityComponent"));
}

void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGCharacter::InteractNative(AActor* Interactor)
{
	// TODO: Temporary call BP event since we can have different NPCs based on this class
	Execute_Interact(this, Interactor);
}

FText ARPGCharacter::GetNameNative() const
{
	return GetCharacterName();
}

