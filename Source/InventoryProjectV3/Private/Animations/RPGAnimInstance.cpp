// Oleksandr Tkachov 2022-2024


#include "Animations/RPGAnimInstance.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/RPGHealth_Component.h"
#include "GameFramework/CharacterMovementComponent.h"

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable CVarDebugDrawIKFeetTraces(TEXT("DebugDrawIKFeetTraces"), 0, TEXT("Enable to draw debug traces for IK Feet system."));
#endif

URPGAnimInstance::URPGAnimInstance()
{
	// Generic
	Character = nullptr;
	bDead = false;
	bMounted = false;
	bFalling = false;
	MountedRootOffset = FVector(0.f, 0.f, 55.f);
	MovementSpeed = 0.f;

	// IK Feet 
	bIKFeetEnabled = true;
	IKHipOffset = 0.f;
	IKFeetAlpha = 0.f;
	IKInterpolationSpeed = 15.f;
	IKTraceDifferenceLimit = 50.f;
	IKLeftFootEffector = FVector::Zero();
	IKRightFootEffector = FVector::Zero();
	IKRightFootOffset = 0.f;
	IKLeftFootOffset = 0.f;
	IKRightFootSocketName = "RightFootSocket";
	IKLeftFootSocketName = "LeftFootSocket";

	// Head rotation
	bHeadRotationEnabled = true;
	HeadTargetRotation = FRotator(0.f);
	MaxHeadYawRotation = 70.f;
	MaxHeadPitchRotation = 35.f;
	HeadRotationInterpolationSpeed = 5.f;
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
		UpdateHeadRotation();
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

#if !UE_BUILD_SHIPPING
		if (CVarDebugDrawIKFeetTraces.GetValueOnGameThread() > 0)
		{
			DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, false, 0.f, 0, 1.f);
			DrawDebugSphere(GetWorld(), OutHit.Location, 5.f, 12, FColor::Green, false, 0.f, 0, 1.f);
		}
#endif
	}
	else
	{
		OutTraceHitLocation = FVector::Zero();
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

	FVector RightFootTraceLocation = FVector::Zero();
	FVector LeftFootTraceLocation = FVector::Zero();

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
	const float TempHipOffset = (TraceLocationDifferenceZ < IKTraceDifferenceLimit) ? (TraceLocationDifferenceZ * -0.5f) : 0.f;

	IKHipOffset = FMath::FInterpTo(IKHipOffset, TempHipOffset, GetDeltaSeconds(), IKInterpolationSpeed);
}

void URPGAnimInstance::UpdateHeadRotation()
{
	if (!bHeadRotationEnabled)
	{
		// Reset head rotation in case head rotation was disabled in runtime (somehow?)
		if (HeadTargetRotation != FRotator(0.f))
		{
			HeadTargetRotation = FRotator(0.f);
		}

		return;
	}

	if (IsHeadRotationRestricted())
	{
		// Reset head rotation
		HeadTargetRotation = FMath::RInterpTo(HeadTargetRotation, FRotator(0.f), GetDeltaSeconds(), HeadRotationInterpolationSpeed);
		return;
	}

	const FRotator CharacterControlRotation = Character->GetControlRotation();
	const FRotator AngleRotator = FRotator(0.f, GetYawRotationDifference(), CharacterControlRotation.Pitch);

	const float NewPitch = FMath::ClampAngle(AngleRotator.Roll, -MaxHeadPitchRotation, MaxHeadPitchRotation);
	const float NewYaw = FMath::ClampAngle(AngleRotator.Yaw, -MaxHeadYawRotation, MaxHeadYawRotation);

	const FRotator NewTarget = FRotator(NewPitch, NewYaw, 0.f);

	// Interpolate to new target based on clamped control rotation
	HeadTargetRotation = FMath::RInterpTo(HeadTargetRotation, NewTarget, GetDeltaSeconds(), HeadRotationInterpolationSpeed);
}

bool URPGAnimInstance::IsHeadRotationRestricted() const
{
	if (FMath::Abs(GetYawRotationDifference()) > MaxHeadYawRotation + 360.f)
	{
		// More precise control over head rotation (prevents head rotating when looking back while character is facing the camera)
		return true;
	}

	if (GetYawRotationDifference() > MaxHeadYawRotation && GetYawRotationDifference() < (360.f - MaxHeadYawRotation))
	{
		// Physically cannot rotate head that much
		return true;
	}

	if (MovementSpeed > Character->GetStealthedMaxWalkSpeed())
	{
		return true;
	}

	return false;
}

float URPGAnimInstance::GetYawRotationDifference() const
{
	const FRotator CharacterControlRotation = Character->GetControlRotation();
	const FRotator CharacterRotation = Character->GetActorRotation();

	return CharacterControlRotation.Yaw - CharacterRotation.Yaw;
}
