// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGAbilityComponent.h"

#include "Abilities/RPGAbilityBase.h"
#include "Abilities/RPGAbilityDefinitionData.h"
#include "Camera/CameraComponent.h"
#include "Characters/RPGPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"

URPGAbilityComponent::URPGAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TemplateAbilityDefinitions.Empty();
}

void URPGAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
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

FTimerHandle URPGAbilityComponent::SetTimerForCastAbility(URPGAbilityBase* Ability, FRPGTargetData TargetData)
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
	
	if (!Ability->AbilityDefinition)
	{
		return 0.0f;
	}
	
	const float RemainingCooldown = FMath::Clamp(Ability->CooldownEndTime - GetWorld()->GetTimeSeconds(), 0.f, Ability->AbilityDefinition->Cooldown);
	return RemainingCooldown;	
}

float URPGAbilityComponent::GetCooldownPercentForAbility(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return 0.0f;
	}
	
	if (!Ability->AbilityDefinition)
	{
		return 0.0f;
	}
	
	const float RemainingCooldownPercent = FMath::Clamp(FMath::Max(0, Ability->CooldownEndTime - GetWorld()->GetTimeSeconds()) / Ability->AbilityDefinition->Cooldown, 0.0f, 1.0f);
	return RemainingCooldownPercent;
}

float URPGAbilityComponent::GetCooldownDurationForAbility(URPGAbilityBase* Ability) const
{
	if (!Ability)
	{
		return 0.0f;
	}
	
	if (!Ability->AbilityDefinition)
	{
		return 0.0f;
	}
	
	return Ability->AbilityDefinition->Cooldown;
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
	
	if (!NewAbilityDefinition->AbilityClass)
	{
		return;
	}
	
	TemplateAbilityDefinitions.Add(NewAbilityDefinition);
	
	URPGAbilityBase* NewAbility = NewObject<URPGAbilityBase>(this, NewAbilityDefinition->AbilityClass);
	if (!NewAbility)
	{
		return;
	}
	
	NewAbility->InitAbility(this, GetOwner(), NewAbilityDefinition);
	
	SpawnedAbilityDefinitions.Add(NewAbilityDefinition, NewAbility);
	
	OnAbilityAdded.Broadcast(NewAbilityDefinition);
}
