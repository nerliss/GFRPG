// Oleksandr Tkachov 2021-2026


#include "Abilities/Archetypes/RPGAbilitySummon.h"

#include "Abilities/RPGTargetingPreviewActor.h"

void URPGAbilitySummon::InitAbility(URPGAbilityComponent* InAbilityComponent, AActor* InOwnerActor,
                                    URPGAbilityDefinitionData* InAbilityDefinitionData)
{
	Super::InitAbility(InAbilityComponent, InOwnerActor, InAbilityDefinitionData);
	
	SummonAbilityDefinitionData = Cast<URPGSummonAbilityDefinitionData>(InAbilityDefinitionData);
}

bool URPGAbilitySummon::UseAbility(FRPGTargetData TargetData)
{
	if (!Super::UseAbility(TargetData))
	{
		return false;
	}
	
	if (!SummonAbilityDefinitionData)
	{
		return false;
	}
	
	if (!AbilityComponent)
	{
		return false;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(TargetData.HitResult.Location);
	
	AActor* SpawnedActor = AbilityComponent->SpawnSummonActor(SummonAbilityDefinitionData->ActorToSpawn, SpawnTransform);
	return IsValid(SpawnedActor);
}

void URPGAbilitySummon::OnCastComplete(FRPGTargetData TargetData)
{
	Super::OnCastComplete(TargetData);
	
	if (!AbilityComponent)
	{
		return;
	}
	
	if (!SummonAbilityDefinitionData)
	{
		return;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(TargetData.HitResult.Location);
	
	AbilityComponent->SpawnSummonActor(SummonAbilityDefinitionData->ActorToSpawn, SpawnTransform);
	
	switch (SummonAbilityDefinitionData->TargetingFlow)
	{
	case EAbilityTargetingFlow::HoldRelease:
		{
			// TODO: Implement the flow
			break;
		}
	case EAbilityTargetingFlow::Instant:
		{
			// TODO: Review system design - might not be needed here
			break;
		}
	case EAbilityTargetingFlow::PreviewConfirm:
		{
			if (AbilityComponent->TargetingPreviewActor)
			{
				AbilityComponent->TargetingPreviewActor->Destroy();
				AbilityComponent->TargetingPreviewActor = nullptr;
				AbilityComponent->TargetingPreviewActorMIDs.Empty();
				break;
			}
		}
	default:
		break;
	}
}

void URPGAbilitySummon::OnCastInterrupted(FRPGTargetData TargetData, EAbilityInterruptReason Reason)
{
	// TODO: Might not need super in these functions
	Super::OnCastInterrupted(TargetData, Reason);
	
	// TODO: Wrap in a function to reuse
	if (!AbilityComponent)
	{
		return;
	}
	
	if (!SummonAbilityDefinitionData)
	{
		return;
	}
	
	switch (SummonAbilityDefinitionData->TargetingFlow)
	{
	case EAbilityTargetingFlow::HoldRelease:
		{
			// TODO: Implement the flow
			break;
		}
	case EAbilityTargetingFlow::Instant:
		{
			// TODO: Review system design - might not be needed here
			break;
		}
	case EAbilityTargetingFlow::PreviewConfirm:
		{
			if (AbilityComponent->TargetingPreviewActor)
			{
				AbilityComponent->TargetingPreviewActor->Destroy();
				AbilityComponent->TargetingPreviewActor = nullptr;
				AbilityComponent->TargetingPreviewActorMIDs.Empty();
				break;
			}
		}
	default:
		break;
	}
}

void URPGAbilitySummon::OnToggleStarted(FRPGTargetData TargetData)
{
	if (!AbilityComponent)
	{
		return;
	}
	
	if (!SummonAbilityDefinitionData)
	{
		return;
	}
	
	if (!OwnerActor)
	{
		return;
	}
		
	ToggleSpawnedActor = AbilityComponent->SpawnSummonActor(SummonAbilityDefinitionData->ActorToSpawn, OwnerActor->GetActorTransform());
}

void URPGAbilitySummon::OnToggleEnded(FRPGTargetData TargetData)
{
	if (!ToggleSpawnedActor)
	{
		return;
	}
	
	ToggleSpawnedActor->Destroy();
}

void URPGAbilitySummon::OnTargetingPreviewStarted(FRPGTargetData TargetData)
{
	if (!AbilityComponent)
	{
		return;
	}
	
	AbilityComponent->SpawnPreviewActor(this);
}

void URPGAbilitySummon::OnTargetingPreviewUpdated(FRPGTargetData TargetData)
{
	if (!AbilityComponent)
	{
		return;
	}
	
	if (!AbilityComponent->TargetingPreviewActor)
	{
		return;
	}
	
	AbilityComponent->TargetingPreviewActor->SetActorLocation(TargetData.HitResult.Location);
}

void URPGAbilitySummon::OnTargetingPreviewCanceled(FRPGTargetData TargetData)
{
	if (!AbilityComponent)
	{
		return;
	}
	
	if (!AbilityComponent->TargetingPreviewActor)
	{
		return;
	}

	// TODO: Rewrite into function (this code block reused 3 times)
	AbilityComponent->TargetingPreviewActor->Destroy();
	AbilityComponent->TargetingPreviewActor = nullptr;
	AbilityComponent->TargetingPreviewActorMIDs.Empty();
}

void URPGAbilitySummon::OnTargetingPreviewConfirmed(FRPGTargetData TargetData)
{
}
