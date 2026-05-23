// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGAbilityComponent.h"

#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "Abilities/RPGAbilityDefinitionData.h"
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
}

void URPGAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilities();
	
	auto RPGCharacter = Cast<ARPGCharacter>(GetOwner());
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
	return 0.f;
}

float URPGAbilityComponent::GetChannelRemainingTimeForAbility(URPGAbilityBase* Ability) const
{
	return 0.f;
}

void URPGAbilityComponent::StartCast(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
}

void URPGAbilityComponent::FinishCast(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
}

void URPGAbilityComponent::InterruptCast(URPGAbilityBase* Ability, EAbilityInterruptReason Reason,
	FRPGTargetData TargetData)
{
}

float URPGAbilityComponent::GetCastDurationPercentForAbility(URPGAbilityBase* Ability) const
{
	return 0.f;
}

float URPGAbilityComponent::GetCastRemainingTimeForAbility(URPGAbilityBase* Ability) const
{
	return 0.f;
}

void URPGAbilityComponent::StartToggle(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
}

void URPGAbilityComponent::StopToggle(URPGAbilityBase* Ability, FRPGTargetData TargetData)
{
}

void URPGAbilityComponent::BeginTargetingPreview(URPGAbilityBase* Ability)
{
}

void URPGAbilityComponent::UpdateTargetingPreview()
{
}

void URPGAbilityComponent::CancelTargetingPreview()
{
}

void URPGAbilityComponent::ConfirmTargetingPreview()
{
}

void URPGAbilityComponent::SpawnPreviewActor(URPGAbilitySummon* SummonAbility)
{
}

AActor* URPGAbilityComponent::SpawnSummonActor(TSubclassOf<AActor> ClassToSpawn, FTransform SpawnTransform)
{
	return nullptr;
}

void URPGAbilityComponent::TryUsingAbility(int32 AbilityArrayIndex)
{
}

void URPGAbilityComponent::TryInterruptingActiveAbilities()
{
}

void URPGAbilityComponent::StartCooldown(URPGAbilityBase* Ability)
{
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
