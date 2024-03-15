// Oleksandr Tkachov 2022-2024


#include "Animations/RPGAnimInstance.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/RPGHealth_Component.h"
#include "GameFramework/CharacterMovementComponent.h"

URPGAnimInstance::URPGAnimInstance()
{
	Character = nullptr;

	bDead = false;
	bMounted = false;
	bFalling = false;
	bIKFeetEnabled = true;

	MovementSpeed = 0.f;
	IKHipOffset = 0.f;

	IKLeftFootEffector = FVector();
	IKRightFootEffector = FVector();

	IKRightFootOffset = 0.f;
	IKLeftFootOffset = 0.f;

	IKRightFootSocketName = "RightFootSocket";
	IKLeftFootSocketName = "LeftFootSocket";
}

void URPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ARPGPlayerCharacter>(TryGetPawnOwner());
}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character)
	{
		bDead = Character->HPComp->bDiedAlready;
		bMounted = Character->bMounted;
		bFalling = Character->GetCharacterMovement()->IsFalling();
		MovementSpeed = Character->GetVelocity().Size();
		
		UpdateIKFeet();
	}
}

void URPGAnimInstance::CalculateIKFootTrace(const FName SocketName, const float TraceDistance, FVector& OutTraceHitLocation, float& OutFootTraceOffset)
{
	auto* CharacterMesh = Character->GetMesh();
	if (!CharacterMesh)
	{
		return;
	}

	const FVector SocketLocation = CharacterMesh->GetSocketLocation(SocketName);
	const FVector StartLoc = FVector(SocketLocation.X, SocketLocation.Y, Character->GetActorLocation().Z);
	const FVector EndLoc = FVector(SocketLocation.X, SocketLocation.Y, SocketLocation.Z - TraceDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	FHitResult OutHit;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLoc, EndLoc, ECC_Visibility, Params))
	{
		OutTraceHitLocation = OutHit.Location;
		OutFootTraceOffset = (OutHit.Location - CharacterMesh->GetComponentLocation()).Z - IKHipOffset;
	}
	else
	{
		OutTraceHitLocation = FVector();
		OutFootTraceOffset = 0.f;
	}
}

bool URPGAnimInstance::ShouldUpdateIKFeet() const
{
	if (bDead || bMounted || bFalling)
	{
		return false;
	}

	return true;
}

void URPGAnimInstance::UpdateIKFeet()
{
	if (!bIKFeetEnabled)
	{
		return;
	}

	if (!ShouldUpdateIKFeet())
	{
		return;
	}

	const float TraceDistance = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector RightFootTraceLocation = FVector();
	FVector LeftFootTraceLocation = FVector();

	CalculateIKFootTrace(IKRightFootSocketName, TraceDistance, RightFootTraceLocation, IKRightFootOffset);
	CalculateIKFootTrace(IKLeftFootSocketName, TraceDistance, LeftFootTraceLocation, IKLeftFootOffset);

	// MyTODO: Implement interpolation
	//IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, );
	IKRightFootEffector = FVector(0.f, 0.f, IKRightFootOffset);
	IKLeftFootEffector = FVector(0.f, 0.f, IKLeftFootOffset);

	const FVector TraceLocationDifference = RightFootTraceLocation - LeftFootTraceLocation;
	const float TraceLocationDifferenceZ = FMath::Abs(TraceLocationDifference.Z);

	// MyTODO: Implement interpolation
	IKHipOffset = (TraceLocationDifferenceZ < 50.f) ? (TraceLocationDifferenceZ * -0.5f) : 0.f;
}
