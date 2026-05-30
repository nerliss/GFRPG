// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGAbilityComponent.h"

#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "Abilities/RPGAbilityDefinitionData.h"
#include "Abilities/RPGTargetingPreviewActor.h"
#include "Abilities/Archetypes/RPGAbilitySummon.h"
#include "Camera/CameraComponent.h"
#include "Characters/RPGPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

URPGAbilityComponent::URPGAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TemplateAbilityDefinitions.Empty();
	SpawnedAbilityDefinitions.Empty();
	ActiveAbilityUpdateTimer = FTimerHandle();
	ActiveChannels.Empty();
	ActiveCast = nullptr;
	bIsInTargetingPreview = false;
	PendingAbility = nullptr;
	TargetingPreviewTimer = FTimerHandle();
	TargetingPreviewUpdateRate = 0.1f;
	TargetingPreviewActorClass = nullptr;
}

void URPGAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilities();
	
	ARPGCharacter* RPGCharacter = Cast<ARPGCharacter>(GetOwner());
	if (RPGCharacter)
	{
		RPGCharacter->OnCharacterMoved.AddDynamic(this, &URPGAbilityComponent::OnOwnerCharacterMoved);
	}
}

void URPGAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGAbilityComponent::TraceForTargetData(const float InTraceLength, URPGAbilityBase* Ability, FRPGTargetData& OutTargetData)
{
	const ARPGPlayerCharacter* Owner = Cast<ARPGPlayerCharacter>(GetOwner());
	if (!Owner)
	{
		return;
	}
	
	const UCameraComponent* OwnerCamera = Owner->CameraComp;
	const USpringArmComponent* OwnerSpringArm = Owner->SpringArmComp;
	
	if (!OwnerCamera || !OwnerSpringArm)
	{
		return;
	}
		
	const FVector StartLoc = OwnerCamera->GetComponentLocation() + OwnerCamera->GetForwardVector() * OwnerSpringArm->TargetArmLength; // Compensate for camera distance
	const FVector EndLoc = (StartLoc + (OwnerCamera->GetForwardVector() * InTraceLength));
	constexpr ECollisionChannel ECC_Channel = ECC_Visibility;
	
	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	const bool bHitResult = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Channel, Params);
	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Purple, false, 4.f, 0, 2.f);
	
	OutTargetData.HitResult = HitResult;
	
	if (!bHitResult && Ability && Ability->AbilityDefinition && Ability->AbilityDefinition->bDoGroundTrace)
	{
		FHitResult ProjectToGroundHitResult;

		const FVector ProjectToGroundStart = EndLoc;
		const FVector ProjectToGroundEnd = EndLoc - FVector::UpVector * Ability->AbilityDefinition->GroundTraceDistance;

		GetWorld()->LineTraceSingleByChannel(ProjectToGroundHitResult, ProjectToGroundStart, ProjectToGroundEnd, ECC_Channel, Params);
		DrawDebugLine(GetWorld(), ProjectToGroundStart, ProjectToGroundEnd, FColor::Emerald, false, 4.f, 0, 2.f);

		OutTargetData.HitResult = ProjectToGroundHitResult;
	}
	
	UE_LOG(LogTemp, Log, TEXT("TargetData.HitLocation = %s, TargetData.TraceEnd = %s, EndLoc = %s"), *OutTargetData.HitResult.Location.ToString(), *OutTargetData.HitResult.TraceEnd.ToString(), *EndLoc.ToString());
}

FRPGTargetData URPGAbilityComponent::TraceForTargetData(const float InTraceLength, URPGAbilityBase* Ability)
{
	FRPGTargetData TargetData;
	TraceForTargetData(InTraceLength, Ability, TargetData);
	return TargetData;
}

void URPGAbilityComponent::SetTimerForAbilityCooldownExpiration(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	if (!Ability->GetAbilityDefinition())
	{
		return;
	}
	
	const float AbilityCooldown = Ability->GetAbilityDefinition()->Cooldown;
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("OnCooldownTimerExpired"), Ability);
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AbilityCooldown, false);
}

void URPGAbilityComponent::OnCooldownTimerExpired(URPGAbilityBase* Ability)
{
	OnAbilityCooldownEnded.Broadcast(Ability);
}

FTimerHandle URPGAbilityComponent::SetTimerForCastAbility(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
	if (!Ability)
	{
		return FTimerHandle();
	}
	
	if (!Ability->GetAbilityDefinition())
	{
		return FTimerHandle();
	}
	
	const float AbilityCastTime = Ability->GetAbilityDefinition()->CastParams.CastTime;
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("OnCastFinished"), Ability, TargetData);
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, AbilityCastTime, false);
	
	return TimerHandle;
}

float URPGAbilityComponent::GetCooldownRemainingForAbility(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return 0.0f;
	}
	
	if (!Ability->GetAbilityDefinition())
	{
		return 0.0f;
	}
	
	const float RemainingCooldown = FMath::Clamp(Ability->CooldownEndTime - GetWorld()->GetTimeSeconds(), 0.f, Ability->GetAbilityDefinition()->Cooldown);
	return RemainingCooldown;	
}

float URPGAbilityComponent::GetCooldownPercentForAbility(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return 0.0f;
	}
	
	if (!Ability->GetAbilityDefinition())
	{
		return 0.0f;
	}
	
	const float RemainingCooldownPercent = FMath::Clamp(FMath::Max(0, Ability->CooldownEndTime - GetWorld()->GetTimeSeconds()) / Ability->GetAbilityDefinition()->Cooldown, 0.0f, 1.0f);
	return RemainingCooldownPercent;
}

float URPGAbilityComponent::GetCooldownDurationForAbility(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return 0.0f;
	}
	
	if (!Ability->GetAbilityDefinition())
	{
		return 0.0f;
	}
	
	return Ability->GetAbilityDefinition()->Cooldown;
}

TArray<FHitResult> URPGAbilityComponent::QuerySphereTargets(FVector SweepStart, FVector SweepEnd, float SweepRadius)
{
	TArray<FHitResult> HitResults;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	
	GetWorld()->SweepMultiByChannel(HitResults, SweepStart, SweepEnd, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SweepRadius), Params);
	return HitResults;
}

FHitResult URPGAbilityComponent::QueryLinetrace(FVector Start, FVector End)
{
	FHitResult HitResult;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	return HitResult;
}

void URPGAbilityComponent::StartChannel(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
	if (!Ability)
	{
		return;
	}
	
	const URPGAbilityDefinitionData* AbilityDefinition = Ability->GetAbilityDefinition();
	if (!AbilityDefinition)
	{
		return;
	}
	
	const FChannelParams ChannelParams = AbilityDefinition->ChannelParams;
	
	Ability->bIsChanneling = true;
	Ability->ChannelEndTime = GetWorld()->GetTimeSeconds() + ChannelParams.ChannelDuration;
	Ability->ChannelTickPeriod = ChannelParams.TickPeriod;
	Ability->NextTickTime = ChannelParams.bTickOnStart ? GetWorld()->GetTimeSeconds() : GetWorld()->GetTimeSeconds() + ChannelParams.TickPeriod;
	Ability->ActiveAbilityTargetData = TargetData;
	Ability->bTickOnStart = ChannelParams.bTickOnStart;
	Ability->bRequiresHold = ChannelParams.bRequiresButtonHold;
	Ability->bUpdateTargetEachTick = ChannelParams.bUpdateTargetEachTick;
	Ability->bInterruptOnMove = ChannelParams.bInterruptOnMove;
	
	ActiveChannels.Add(Ability);
	
	Ability->OnChannelStart(TargetData);
	
	if (Ability->bTickOnStart)
	{
		UpdateChannels();
	}
	
	GetWorld()->GetTimerManager().SetTimer(ActiveAbilityUpdateTimer, this, &URPGAbilityComponent::UpdateChannels, Ability->ChannelTickPeriod, true);
	
	OnAbilityChannelStarted.Broadcast(Ability);
}

void URPGAbilityComponent::UpdateChannels()
{
	if (ActiveChannels.Num() <= 0)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Error, TEXT("ActiveChannels is empty, function will not be executed"));
		return;
	}
	
	// TODO: Test this out since I might have messed up the loop flow :|
	for (int i = ActiveChannels.Num() - 1; i >= 0; i--)
	{
		URPGAbilityBase*& ActiveChannel = ActiveChannels[i];
		if (!ActiveChannel || !ActiveChannel->GetAbilityDefinition())
		{
			// TODO: We might want to add this ability's index removal from ActiveChannel in case if it's not valid somewhere down the line
			// though this should not happen
			StopChannel(ActiveChannel, EAbilityInterruptReason::Interrupt);
			continue;
		}
		
		if (GetWorld()->GetTimeSeconds() >= ActiveChannel->NextTickTime)
		{
			if (ActiveChannel->bUpdateTargetEachTick)
			{
				ActiveChannel->ActiveAbilityTargetData = TraceForTargetData(ActiveChannel->GetAbilityDefinition()->CastRange, ActiveChannel);
			}
			
			ActiveChannel->OnChannelTick(ActiveChannel->ActiveAbilityTargetData);
			ActiveChannel->NextTickTime += ActiveChannel->ChannelTickPeriod;
			
			if (GetWorld()->GetTimeSeconds() >= ActiveChannel->ChannelEndTime)
			{
				StopChannel(ActiveChannel, EAbilityInterruptReason::DurationEnd);
			}
		}
		else
		{
			StopChannel(ActiveChannel, EAbilityInterruptReason::DurationEnd);
		}
	}
}

void URPGAbilityComponent::StopChannel(URPGAbilityBase* Ability, EAbilityInterruptReason Reason)
{
	if (!Ability)
	{
		return;
	}
	
	if (!Ability->bIsChanneling)
	{
		return;
	}
	
	Ability->bIsChanneling = false;
	
	ActiveChannels.Remove(Ability);
	
	Ability->OnChannelEnd(Ability->ActiveAbilityTargetData, Reason);
	
	GetWorld()->GetTimerManager().ClearTimer(ActiveAbilityUpdateTimer);
	
	OnAbilityChannelStopped.Broadcast(Ability, Reason);
}

bool URPGAbilityComponent::HasActiveAbilities() const
{
	return ActiveChannels.Num() > 0 || ActiveCast;
}

float URPGAbilityComponent::GetChannelDurationPercentForAbility(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return 0.f;
	}
	
	const URPGAbilityDefinitionData* AbilityDefinition = Ability->GetAbilityDefinition();
	if (!AbilityDefinition)
	{
		return 0.f;
	}
	
	return FMath::Clamp(FMath::Max(0.f, Ability->ChannelEndTime - GetWorld()->GetTimeSeconds()) / AbilityDefinition->ChannelParams.ChannelDuration, 0.f, 1.f); 
}

float URPGAbilityComponent::GetChannelRemainingTimeForAbility(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return 0.f;
	}
	
	const URPGAbilityDefinitionData* AbilityDefinition = Ability->GetAbilityDefinition();
	if (!AbilityDefinition)
	{
		return 0.f;
	}
	
	return FMath::Clamp(Ability->ChannelEndTime - GetWorld()->GetTimeSeconds(), 0.f, AbilityDefinition->ChannelParams.ChannelDuration);
}

void URPGAbilityComponent::StartCast(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
	if (!Ability)
	{
		return;
	}
	
	const URPGAbilityDefinitionData* AbilityDefinition = Ability->GetAbilityDefinition();
	if (!AbilityDefinition)
	{
		return;
	}
	
	const FCastParams CastParams = AbilityDefinition->CastParams;
	
	ActiveCast = Ability;
	
	Ability->ActiveAbilityTargetData = TargetData;
	Ability->bInterruptOnMove = CastParams.bInterruptOnMove;
	Ability->bIsCasting = true;
	Ability->CastStartTime = GetWorld()->GetTimeSeconds();
	Ability->CastEndTime = GetWorld()->GetTimeSeconds() + CastParams.CastTime;
	Ability->bLockTargetAtCastStart = CastParams.bLockTargetAtStart;
	Ability->bPayCostOnStart = CastParams.bPayCostOnStart;
	Ability->bStartCooldownOnStart = CastParams.bStartCooldownOnStart;
	
	Ability->OnCastStart(TargetData);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("FinishCast"), Ability,  Ability->bLockTargetAtCastStart ? TargetData : TraceForTargetData(AbilityDefinition->CastRange, Ability));
	
	//FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ActiveAbilityUpdateTimer, TimerDelegate, CastParams.CastTime, false);
	//ActiveAbilityUpdateTimer = SetTimerForCastAbility(Ability, TargetData);
	
	OnAbilityChannelStarted.Broadcast(Ability);
}

void URPGAbilityComponent::FinishCast(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
	if (!Ability)
	{
		return;
	}
	
	Ability->OnCastComplete(TargetData);
	
	Ability->bIsCasting = false;
	
	ActiveCast = nullptr;
	
	OnAbilityChannelStopped.Broadcast(Ability, EAbilityInterruptReason::DurationEnd);
}

void URPGAbilityComponent::InterruptCast(URPGAbilityBase* Ability, EAbilityInterruptReason Reason,
	FRPGTargetData TargetData)
{
	if (!Ability)
	{
		return;
	}
	
	Ability->OnCastInterrupted(TargetData, Reason);
	
	Ability->bIsCasting = false;
	
	ActiveCast = nullptr;
	
	GetWorld()->GetTimerManager().ClearTimer(ActiveAbilityUpdateTimer);
	
	OnAbilityChannelStopped.Broadcast(Ability, Reason);
}

float URPGAbilityComponent::GetCastDurationPercentForAbility(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return 0.f;
	}
	
	const URPGAbilityDefinitionData* AbilityDefinition = Ability->GetAbilityDefinition();
	if (!AbilityDefinition)
	{
		return 0.f;
	}
	
	return FMath::Clamp(FMath::Max(0.f, Ability->CastEndTime - GetWorld()->GetTimeSeconds()) / AbilityDefinition->CastParams.CastTime, 0.f, 1.f);
}

float URPGAbilityComponent::GetCastRemainingTimeForAbility(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return 0.f;
	}
	
	const URPGAbilityDefinitionData* AbilityDefinition = Ability->GetAbilityDefinition();
	if (!AbilityDefinition)
	{
		return 0.f;
	}
	
	return FMath::Clamp(Ability->CastEndTime - GetWorld()->GetTimeSeconds(), 0.f, AbilityDefinition->CastParams.CastTime);
}

void URPGAbilityComponent::StartToggle(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
	if (!Ability)
	{
		return;
	}
	
	Ability->OnToggleStarted(TargetData);
	
	Ability->bIsToggled = true;
	
	OnAbilityToggled.Broadcast(Ability, true);
}

void URPGAbilityComponent::StopToggle(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
	if (!Ability)
	{
		return;
	}
	
	Ability->OnToggleEnded(TargetData);
	
	Ability->bIsToggled = false;
	
	OnAbilityToggled.Broadcast(Ability, false);
}

void URPGAbilityComponent::BeginTargetingPreview(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	if (!Ability->GetAbilityDefinition())
	{
		return;
	}
	
	bIsInTargetingPreview = true;
	
	PendingAbility = Ability;
	
	Ability->OnTargetingPreviewStarted(TraceForTargetData(Ability->GetAbilityDefinition()->CastRange, Ability));
	
	GetWorld()->GetTimerManager().SetTimer(TargetingPreviewTimer, this, &URPGAbilityComponent::UpdateTargetingPreview, TargetingPreviewUpdateRate, true);
	
	OnAbilityTargetingPreviewStarted.Broadcast(Ability);
}

void URPGAbilityComponent::UpdateTargetingPreview()
{
	if (!PendingAbility)
	{
		return;
	}
	
	if (!PendingAbility->GetAbilityDefinition())
	{
		return;
	}
	
	FRPGTargetData TargetData;
	TraceForTargetData(PendingAbility->GetAbilityDefinition()->CastRange, PendingAbility, TargetData);
	
	if (!TargetData.HitResult.bBlockingHit)
	{
		CancelTargetingPreview();
		return;
	}
	
	TargetingPreviewTargetData = TargetData;
	PendingAbility->OnTargetingPreviewUpdated(TargetingPreviewTargetData);
}

void URPGAbilityComponent::CancelTargetingPreview()
{
	if (!bIsInTargetingPreview)
	{
		return;
	}
	
	if (!PendingAbility)
	{
		return;
	}
	
	PendingAbility->OnTargetingPreviewCanceled(TargetingPreviewTargetData);
	OnAbilityTargetingPreviewCanceled.Broadcast(PendingAbility);
	
	bIsInTargetingPreview = false;
	PendingAbility = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(TargetingPreviewTimer);
}

void URPGAbilityComponent::ConfirmTargetingPreview()
{
	if (!bIsInTargetingPreview)
	{
		return;
	}
	
	if (!PendingAbility)
	{
		return;
	}
	
	if (!PendingAbility->GetAbilityDefinition())
	{
		return;
	}
	
	PendingAbility->OnTargetingPreviewConfirmed(TargetingPreviewTargetData);
	OnAbilityTargetingPreviewConfirmed.Broadcast(PendingAbility);

	switch (PendingAbility->GetAbilityDefinition()->ActivationMode)
	{
	case EAbilityActivationMode::Cast:
		{
			// TODO: Incomplete logic
			StartCast(PendingAbility, TargetingPreviewTargetData);
			
			bIsInTargetingPreview = false;
			
			GetWorld()->GetTimerManager().ClearTimer(TargetingPreviewTimer);
			
			PendingAbility = nullptr;
			break;
		}
	case EAbilityActivationMode::Channel:
		{
			break;
		}
	case EAbilityActivationMode::Instant:
		{
			PendingAbility->UseAbility(TargetingPreviewTargetData);
			
			StartCooldown(PendingAbility);
			
			bIsInTargetingPreview = false;
			
			GetWorld()->GetTimerManager().ClearTimer(TargetingPreviewTimer);
			
			if (TargetingPreviewActor)
			{
				TargetingPreviewActor->Destroy();
				TargetingPreviewActor = nullptr;
			}
			
			TargetingPreviewActorMIDs.Empty();
			
			PendingAbility = nullptr;
			break;
		}
	case EAbilityActivationMode::Toggle:
		{
			break;	
		}
		default:
		break;
	}
	
}

void URPGAbilityComponent::SpawnPreviewActor(URPGAbilitySummon* SummonAbility)
{
	if (!SummonAbility)
	{
		return;
	}
	
	URPGSummonAbilityDefinitionData* SummonAbilityDefinitionData = SummonAbility->GetAbilityDefinition();
	if (!SummonAbilityDefinitionData)
	{
		return;
	}
	
	if (!TargetingPreviewActorClass)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Error, TEXT("Can't spawn preview actor since TargetingPreviewActorClass is null"));
		return;
	}
	
	if (TargetingPreviewActor)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Error, TEXT("Failed to spawn a preview actor for ability %s since TargetingPreviewActor already exists (%s)"), *SummonAbility->GetName(), *TargetingPreviewActor->GetName());
		return;
	}
	
	FRPGTargetData TargetData = TraceForTargetData(SummonAbilityDefinitionData->CastRange, SummonAbility);
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(TargetData.HitResult.Location);
	
	TargetingPreviewActor = GetWorld()->SpawnActor<ARPGTargetingPreviewActor>(TargetingPreviewActorClass, SpawnTransform);
	if (!TargetingPreviewActor)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGAbilitySystem, Error, TEXT("Failed to spawn a preview actor for ability %s. Possible reason - TargetingPreviewActorClass is not set (%s)"), *SummonAbility->GetName(), *GetNameSafe(TargetingPreviewActorClass));
		return;
	}
	
	USkeletalMeshComponent* SkeletalMeshComponent = TargetingPreviewActor->SkeletalMesh;
	if (!SkeletalMeshComponent)
	{
		return;
	}
	
	SkeletalMeshComponent->SetSkeletalMesh(SummonAbilityDefinitionData->PreviewMesh);
	SkeletalMeshComponent->SetRelativeScale3D(SummonAbilityDefinitionData->SpawnScale);
	SkeletalMeshComponent->SetAnimInstanceClass(SummonAbilityDefinitionData->PreviewAnimClass);
	
	for (int i = 0; i < SkeletalMeshComponent->GetNumMaterials(); i++)
	{
		TargetingPreviewActorMIDs.Add(SkeletalMeshComponent->CreateDynamicMaterialInstance(i, SummonAbilityDefinitionData->PreviewMaterial));
	}
}

AActor* URPGAbilityComponent::SpawnSummonActor(TSubclassOf<AActor> ClassToSpawn, FTransform SpawnTransform)
{
	if (!ClassToSpawn)
	{
		return nullptr;
	}
	
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	return GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTransform, ActorSpawnParameters);
}

void URPGAbilityComponent::TryUsingAbility(int32 AbilityArrayIndex)
{
	TArray<URPGAbilityBase*> SpawnedAbilities;
	SpawnedAbilityDefinitions.GenerateValueArray(SpawnedAbilities);
	
	if (!SpawnedAbilities.IsValidIndex(AbilityArrayIndex))
	{
		return;
	}
	
	URPGAbilityBase*& Ability = SpawnedAbilities[AbilityArrayIndex];
	if (!Ability)
	{
		return;
	}
	
	const URPGAbilityDefinitionData* AbilityDefinitionData = Ability->GetAbilityDefinition();
	if (!AbilityDefinitionData)
	{
		return;
	}
	
	if (!Ability->CanUseAbility())
	{
		return;
	}

	switch (AbilityDefinitionData->TargetingFlow)
	{
	case EAbilityTargetingFlow::Instant:
		{
			const FRPGTargetData TargetData = TraceForTargetData(AbilityDefinitionData->CastRange, Ability);

			switch (AbilityDefinitionData->ActivationMode)
			{
			case EAbilityActivationMode::Instant:
				{
					Ability->UseAbility(TargetData);
					break;
				}
			case EAbilityActivationMode::Toggle:
				{
					if (Ability->bIsToggled)
					{
						StopToggle(Ability, TargetData);
					}
					else
					{
						StartToggle(Ability, TargetData);
						
						// We don't want to start a cd when toggling an ability 
						OnAbilityUsed.Broadcast(Ability);
						return;
					}
					break;
				}
			case EAbilityActivationMode::Channel:
				{
					StartChannel(Ability, TargetData);
					break;
				}
			case EAbilityActivationMode::Cast:
				{
					StartCast(Ability, TargetData);
					break;
				}
				default:
				break;
			}
			
			StartCooldown(Ability);
			OnAbilityUsed.Broadcast(Ability);
			
			break;
		}
	case EAbilityTargetingFlow::PreviewConfirm:
		{
			BeginTargetingPreview(Ability);
			break;
		}
	case EAbilityTargetingFlow::HoldRelease:
		{
			// TODO: Implement
			break;
		}
		default:
		break;
	}
}

void URPGAbilityComponent::TryInterruptingActiveAbilities()
{
	if (!HasActiveAbilities())
	{
		return;
	}
	
	for (URPGAbilityBase* ActiveChannel : ActiveChannels)
	{
		if (!ActiveChannel)
		{
			continue;
		}
		
		// TODO: Add possible reasons here
		if (ActiveChannel->bInterruptOnMove)
		{
			StopChannel(ActiveChannel, EAbilityInterruptReason::Moved);
		}
	}
	
	if (ActiveCast && ActiveCast->bInterruptOnMove)
	{
		InterruptCast(ActiveCast, EAbilityInterruptReason::Moved, ActiveCast->ActiveAbilityTargetData);
	}
}

void URPGAbilityComponent::StartCooldown(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	if (!Ability->GetAbilityDefinition())
	{
		return;
	}
	
	Ability->CooldownEndTime = GetWorld()->GetTimeSeconds() + Ability->GetAbilityDefinition()->Cooldown;
	
	OnAbilityCooldownStarted.Broadcast(Ability);
	
	// TODO: In blueprints it is stated that this function should be removed upon moving the logic to C++. Investigate
	SetTimerForAbilityCooldownExpiration(Ability);
}

bool URPGAbilityComponent::IsAbilityOnCooldown(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return false;
	}
	
	return !FMath::IsNearlyEqual(GetCooldownRemainingForAbility(Ability), 0.f); 
}

void URPGAbilityComponent::AddAbility(URPGAbilityDefinitionData* NewAbilityDefinition)
{
	if (!NewAbilityDefinition)
	{
		return;
	}
	
	TemplateAbilityDefinitions.Add(NewAbilityDefinition);
	
	SpawnAbilityObject(NewAbilityDefinition);
	
	OnAbilityAdded.Broadcast(NewAbilityDefinition);
}

void URPGAbilityComponent::InitAbilities()
{
	for (URPGAbilityDefinitionData* TemplateAbilityDefinition : TemplateAbilityDefinitions)
	{
		if (!TemplateAbilityDefinition)
		{
			continue;
		}
		
		SpawnAbilityObject(TemplateAbilityDefinition);
	}
}

URPGAbilityBase* URPGAbilityComponent::SpawnAbilityObject(URPGAbilityDefinitionData* AbilityDefinition)
{
	if (!AbilityDefinition || !AbilityDefinition->AbilityClass)
	{
		return nullptr;
	}
	
	URPGAbilityBase* NewAbility = NewObject<URPGAbilityBase>(this, AbilityDefinition->AbilityClass);
	if (!NewAbility)
	{
		return nullptr;
	}
		
	NewAbility->InitAbility(this, GetOwner(), AbilityDefinition);
	SpawnedAbilityDefinitions.Add(AbilityDefinition, NewAbility);
	
	return NewAbility;
}

void URPGAbilityComponent::OnOwnerCharacterMoved()
{
	TryInterruptingActiveAbilities();
}
