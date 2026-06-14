// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGEffectsComponent.h"

#include "Abilities/RPGEffectDefinitionData.h"

URPGEffectsComponent::URPGEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URPGEffectsComponent::ApplyEffect(URPGEffectDefinitionData* EffectDefinition, AActor* Instigator)
{
	if (!EffectDefinition)
	{
		return;
	}
	
	if (EffectDefinition->DurationPolicy == EEffectsDurationPolicy::Instant)
	{
		ExecuteEffect(EffectDefinition, Instigator);
		return;
	}
	
	FRPGActiveEffect* AlreadyActiveEffect = ActiveEffects.FindByPredicate([EffectDefinition](const FRPGActiveEffect& Effect)
	{
		return Effect.Definition == EffectDefinition;
	});
	
	if (AlreadyActiveEffect)
	{
		HandleStacking(*AlreadyActiveEffect, EffectDefinition);
		return;
	}
	
	FRPGActiveEffect NewEffect;
	NewEffect.Definition = EffectDefinition;
	NewEffect.RemainingDuration = EffectDefinition->Duration;
	NewEffect.CurrentStacks = 1;
	
	if (EffectDefinition->bExecuteOnApply)
	{
		ExecuteEffect(EffectDefinition, Instigator);
	}
	
	if (EffectDefinition->DurationPolicy == EEffectsDurationPolicy::Duration)
	{
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &URPGEffectsComponent::OnEffectDurationExpired, EffectDefinition);
		GetWorld()->GetTimerManager().SetTimer(NewEffect.DurationHandle, Delegate, EffectDefinition->Duration, false);
	}
	
	ActiveEffects.Add(NewEffect);
	OnEffectApplied.Broadcast(EffectDefinition);
}

void URPGEffectsComponent::RemoveEffect(URPGEffectDefinitionData* EffectDefinition)
{
	if (!EffectDefinition)
	{
		return;
	}
	
	const int32 Index = ActiveEffects.IndexOfByPredicate([EffectDefinition](const FRPGActiveEffect& Effect)
	{
		return Effect.Definition == EffectDefinition;
	});
	
	if (Index == INDEX_NONE)
	{
		return;
	}
	
	GetWorld()->GetTimerManager().ClearTimer(ActiveEffects[Index].DurationHandle);
	ActiveEffects.RemoveAt(Index);
	OnEffectRemoved.Broadcast(EffectDefinition);
}

bool URPGEffectsComponent::HasEffect(URPGEffectDefinitionData* EffectDefinition) const
{
	return false;
}

float URPGEffectsComponent::GetEffectRemainingDuration(URPGEffectDefinitionData* EffectDefinition) const
{
	return 0.f;
}

void URPGEffectsComponent::BeginPlay()
{
	Super::BeginPlay();
}


void URPGEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGEffectsComponent::ExecuteEffect(URPGEffectDefinitionData* EffectDefinition, AActor* Instigator)
{
}

void URPGEffectsComponent::HandleStacking(FRPGActiveEffect& ExistingEffect,
	URPGEffectDefinitionData* EffectDefinition)
{
	if (!EffectDefinition)
	{
		return;
	}
	
	switch (EffectDefinition->StackPolicy)
	{
	case EEffectsStackPolicy::RefreshDuration:
		{
			
		}
	case EEffectsStackPolicy::Stack:
		{
			
		}
	case EEffectsStackPolicy::StackAndRefreshDuration:
		{
			
		}
	case EEffectsStackPolicy::MAX:
	case EEffectsStackPolicy::NoStack:
		default:
		break;
	}
}

void URPGEffectsComponent::OnEffectDurationExpired(URPGEffectDefinitionData* EffectDefinition)
{
	RemoveEffect(EffectDefinition);
}
