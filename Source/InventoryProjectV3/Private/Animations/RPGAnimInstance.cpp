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
	MountedRootOffset = FVector(0.f, 0.f, 55.f);

	MovementSpeed = 0.f;

	// IK Feet settings
	bIKFeetEnabled = true;
	IKHipOffset = 0.f;
	IKFeetAlpha = 0.f;
	IKInterpolationSpeed = 15.f;
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

void URPGAnimInstance::CalculateIKFeetAlpha()
{
	if (!bIKFeetEnabled)
	{
		IKFeetAlpha = 0.f;
		return;
	}

	// MyTODO: Add interpolation later (in case it is required)
	IKFeetAlpha = ShouldUpdateIKFeet();
}

void URPGAnimInstance::UpdateIKFeet()
{
	// Ensure IKFeetAlpha is always calculated
	ON_SCOPE_EXIT
	{
		CalculateIKFeetAlpha();
	};

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

	float TempIKRightFootOffset = 0.f;
	float TempIKLeftFootOffset = 0.f;

	// Calculate IK Feet traces
	CalculateIKFootTrace(IKRightFootSocketName, TraceDistance, RightFootTraceLocation, TempIKRightFootOffset);
	CalculateIKFootTrace(IKLeftFootSocketName, TraceDistance, LeftFootTraceLocation, TempIKLeftFootOffset);

	// Interpolate between current offset and new offset
	IKRightFootOffset = FMath::FInterpTo(IKRightFootOffset, TempIKRightFootOffset, GetDeltaSeconds(), IKInterpolationSpeed);
	IKLeftFootOffset = FMath::FInterpTo(IKLeftFootOffset, TempIKLeftFootOffset, GetDeltaSeconds(), IKInterpolationSpeed);

	// Set effector target to use directly in AnimGraph
	IKRightFootEffector = FVector(0.f, 0.f, IKRightFootOffset);
	IKLeftFootEffector = FVector(0.f, 0.f, IKLeftFootOffset);

	// Adjust Hip Offset
	const FVector TraceLocationDifference = RightFootTraceLocation - LeftFootTraceLocation;
	const float TraceLocationDifferenceZ = FMath::Abs(TraceLocationDifference.Z);
	const float TempHipOffset = (TraceLocationDifferenceZ < 50.f) ? (TraceLocationDifferenceZ * -0.5f) : 0.f;

	IKHipOffset = FMath::FInterpTo(IKHipOffset, TempHipOffset, GetDeltaSeconds(), IKInterpolationSpeed);
}
