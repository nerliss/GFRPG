// Oleksandr Tkachov 2021-2026


#include "Abilities/Archetypes/RPGAbilityBase.h"

#include "Abilities/RPGAbilityComponent.h"
#include "Abilities/RPGAbilityDefinitionData.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

URPGAbilityBase::URPGAbilityBase()
{
	AbilityDefinition = nullptr;
}

void URPGAbilityBase::InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor,
                                  URPGAbilityDefinitionData* InAbilityDefinitionData)
{
	AbilityComponent = InAbilityComponent;
	OwnerActor = InOwnerActor;
	AbilityDefinition = InAbilityDefinitionData;
}

bool URPGAbilityBase::CanUseAbility()
{
	if (!AbilityComponent)
	{
		return false;
	}
	
	if (!AbilityDefinition)
	{
		return false;
	}
	
	if (!OwnerActor)
	{
		return false;
	}
	
	if (AbilityComponent->IsAbilityOnCooldown(this))
	{
		return false;
	}
	
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
	
	const bool bCasterMoving = !OwnerActor->GetVelocity().IsNearlyZero();
	
	switch (AbilityDefinition->ActivationMode)
	{
		// Instant and Toggle have no additional conditions as of now
	case EAbilityActivationMode::Instant:
	case EAbilityActivationMode::Toggle:
		return true;
		
	case EAbilityActivationMode::Cast:
		return !(bCasterMoving && AbilityDefinition->CastParams.bInterruptOnMove);
		
	case EAbilityActivationMode::Channel:
		return !(bCasterMoving && AbilityDefinition->ChannelParams.bInterruptOnMove);
	
	default:
		return false;
	}
}

bool URPGAbilityBase::UseAbility(FRPGTargetData TargetData)
{
	if (!AbilityComponent)
	{
		return false;
	}
	
	if (!AbilityDefinition)
	{
		return false;
	}
	
	if (!OwnerActor)
	{
		return false;
	}
	
	if (!CanUseAbility())
	{
		return false;
	}
	
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
	AbilityComponent->OnAbilityUsed.Broadcast(this);
	return true;
}

void URPGAbilityBase::OnChannelStart(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnChannelTick(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnChannelEnd(FRPGTargetData TargetData, EAbilityInterruptReason Reason)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnCastStart(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnCastComplete(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnCastInterrupted(FRPGTargetData TargetData, EAbilityInterruptReason Reason)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnToggleStarted(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnToggleEnded(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnTargetingPreviewStarted(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnTargetingPreviewUpdated(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnTargetingPreviewConfirmed(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}

void URPGAbilityBase::OnTargetingPreviewCanceled(FRPGTargetData TargetData)
{
	LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Warning, TEXT("Base implementation used"));
}
