// Oleksandr Tkachov 2022-2024


#include "Creatures/RPGMountBase.h"

#include "Camera/CameraComponent.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Utility/LogDefinitions.h"

ARPGMountBase::ARPGMountBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f; 
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;

	// SpringArm (Camera boom) component configuration
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->TargetArmLength = 500.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 65.f));

	// Camera component configuration
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	AttachmentSocket = "DriverSeat";
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

	PlayerInputComponent->BindAction("Dismount", IE_Pressed, this, &ARPGMountBase::OnDismount);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGMountBase::OnForwardMoved);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGMountBase::OnRightMoved);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPGMountBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPGMountBase::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ARPGMountBase::InteractNative(AActor* Interactor)
{
	OnMount(Interactor);
}

FText ARPGMountBase::GetNameNative() const
{
	return Name;
}

void ARPGMountBase::OnForwardMoved(const float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	if (!bMounted)
	{
		return;
	}

	// Define rotation to get forward vector from
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ARPGMountBase::OnRightMoved(const float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	if (!bMounted)
	{
		return;
	}

	// Find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ARPGMountBase::TurnAtRate(const float Rate)
{
	AddControllerYawInput(Rate * 100 * GetWorld()->GetDeltaSeconds());
}

void ARPGMountBase::LookUpAtRate(const float Rate)
{
	AddControllerPitchInput(Rate * 100 * GetWorld()->GetDeltaSeconds());
}

void ARPGMountBase::OnDismount()
{
	if (!bMounted)
	{
		return;
	}

	bMounted = false;

	GetCharacterMovement()->StopMovementImmediately();
	
	auto* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	// MyTODO: Will probably need to be changed to a loop if there are more than one attached actors (or better yet - to a direct reference to a rider (and may be passengers))
	auto* InteractorCharacter = Cast<ARPGPlayerCharacter>(AttachedActors[0]);
	if (!InteractorCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractorCharacter is invalid"));
		return;
	}

	InteractorCharacter->bMounted = false;

	const FDetachmentTransformRules DetachmentRules { EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepWorld, false  };
	InteractorCharacter->DetachFromActor(DetachmentRules);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	PC->Possess(InteractorCharacter);

	InteractorCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractorCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractorCharacter->SetActorLocation(GetActorLocation() + GetActorRightVector() * 100.f);
	InteractorCharacter->SetPOV(EPlayerPOV::ThirdPerson);

	// Doesn't work as intended
	PC->SetViewTargetWithBlend(InteractorCharacter, 1.f, EViewTargetBlendFunction::VTBlend_Cubic);
}

void ARPGMountBase::OnMount(AActor* Interactor)
{
	auto* InteractorCharacter = Cast<ARPGPlayerCharacter>(Interactor);
	if (!InteractorCharacter)
	{
		return;
	}

	auto* PC = Cast<APlayerController>(Interactor->GetInstigatorController());
	if (!PC)
	{
		return;
	}

	bMounted = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	const FAttachmentTransformRules AttachmentRules{ FAttachmentTransformRules::SnapToTargetNotIncludingScale };

	InteractorCharacter->AttachToComponent(GetMesh(), AttachmentRules, AttachmentSocket);
	InteractorCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractorCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractorCharacter->bMounted = true;
	InteractorCharacter->GetCharacterMovement()->StopMovementImmediately();
	InteractorCharacter->SetPOV(EPlayerPOV::ThirdPerson);

	// Doesn't work as intended
	PC->SetViewTargetWithBlend(this, 1.f, EViewTargetBlendFunction::VTBlend_Cubic);

	PC->Possess(this);
}

