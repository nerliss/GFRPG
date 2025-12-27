// Oleksandr Tkachov 2022-2026


#include "Abilities/RPGAbilityComponent.h"

#include "Abilities/RPGAbilityBase.h"
#include "Abilities/RPGAbilityDefinitionData.h"

URPGAbilityComponent::URPGAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URPGAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URPGAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGAbilityComponent::SetTimerForAbilityCooldownExpiration(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	if (!Ability->AbilityDefinition)
	{
		return;
	}
	
	const float AbilityCooldown = Ability->AbilityDefinition->Cooldown;
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("OnCooldownTimerExpired"), Ability);
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AbilityCooldown, false);
}

void URPGAbilityComponent::OnCooldownTimerExpired(URPGAbilityBase* Ability)
{
	OnAbilityCooldownEnded.Broadcast(Ability);
}

FTimerHandle URPGAbilityComponent::SetTimerForCastAbility(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return FTimerHandle();
	}
	
	if (!Ability->AbilityDefinition)
	{
		return FTimerHandle();
	}
	
	const float AbilityCastTime = Ability->AbilityDefinition->CastParams.CastTime;
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("OnCastFinished"), Ability);
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AbilityCastTime, false);
	
	return TimerHandle;
}

