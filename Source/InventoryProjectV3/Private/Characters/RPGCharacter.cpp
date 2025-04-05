// Oleksandr Tkachov 2022-2025


#include "Characters/RPGCharacter.h"

ARPGCharacter::ARPGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterName = FText::FromString("Default Character");
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
	return CharacterName;
}

