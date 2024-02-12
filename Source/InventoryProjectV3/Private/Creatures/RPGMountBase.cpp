// Oleksandr Tkachov 2022-2024


#include "Creatures/RPGMountBase.h"

#include "Components/CapsuleComponent.h"
#include "Utility/LogDefinitions.h"

ARPGMountBase::ARPGMountBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARPGMountBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPGMountBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPGMountBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// MyTODO: Setup input for mount logic
	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGMountBase::OnForwardMoved);

}

void ARPGMountBase::InteractNative(AActor* Interactor)
{
	auto* InteractorCharacter = Cast<ACharacter>(Interactor);
	if (!InteractorCharacter)
	{
		return;
	}

	const FAttachmentTransformRules AttachmentRules {FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false)};

	InteractorCharacter->AttachToActor(this, AttachmentRules, TEXT("DriverSeat"));

	bMounted = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractorCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractorCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EnableInput(Cast<APlayerController>(InteractorCharacter->GetInstigatorController()));
}

FText ARPGMountBase::GetNameNative() const
{
	return Name;
}

void ARPGMountBase::OnForwardMoved(const float Value)
{
	UE_LOG(LogRPGMount, Warning, TEXT("Forward moved: %f"), Value);
}

