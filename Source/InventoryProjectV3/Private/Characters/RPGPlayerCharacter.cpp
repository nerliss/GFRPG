// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/RPGXP_Component.h"
#include "Components/RPGHealth_Component.h"
#include "Components/RPGReputation_Component.h"
#include "Components/RPGInventory_Component.h"
#include "Components/PointLightComponent.h"
#include "AkComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Widgets/RPGInteractionPrompt_Widget.h"
#include "Widgets/RPGHUD_Widget.h"
#include "Blueprint/UserWidget.h"
#include "Utility/Utility.h"
#include "Interfaces/RPGInteract_Interface.h"
#include "DamageTypes/DamageTypeEnviromental.h"
#include "DataAssets/CharacterSoundCollection.h"

DEFINE_LOG_CATEGORY(LogRPGPlayerCharacter);

// Sets default values
ARPGPlayerCharacter::ARPGPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set initial capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Character movement configuration
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 600.f; // default is 600
	GetCharacterMovement()->AirControl = 0.2f; // default is 0.2

	// SpringArm (Camera boom) component configuration
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent(), "head");
	SpringArmComp->TargetArmLength = 500.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset = FVector(0.f, 80.f, 0.f); // Give camera more Skyrim-like style
	SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 65.f));

	// Camera component configuration
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	// Create XP component
	XPComp = CreateDefaultSubobject<URPGXP_Component>(TEXT("XPComp"));

	// Create HP component
	HPComp = CreateDefaultSubobject<URPGHealth_Component>(TEXT("HPComp"));
	HPComp->SetMaxHealth(100.f);
	HPComp->SetCurrentHealth(HPComp->GetMaxHealth());

	// Create Reputation component
	ReputationComp = CreateDefaultSubobject<URPGReputation_Component>(TEXT("ReputationComp"));

	// Create inventory component
	InventoryComp = CreateDefaultSubobject<URPGInventory_Component>(TEXT("InventoryComp"));

	// Create audio component
	AkComp = CreateDefaultSubobject<UAkComponent>(TEXT("AkComp"));
	AkComp->SetupAttachment(GetRootComponent()); // Any Ak component must be attached, otherwise it doesn't work

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Set turn rates
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Camera
	MaxTargetBoomLength = SpringArmComp->TargetArmLength;
	MinTargetBoomLength = 0.f;

	// POV
	PlayerPOV = EPlayerPOV::ThirdPerson;

	// Dialog
	bInDialog = false;

	// Fall damage
	bCanGetDamagedFromFalling = true;
	FallDamageMinimalThreshold = 1300.f;
	FallDamageMultiplier = 3.f;

	// Sounds
	CharacterSoundCollection = nullptr;

	bStealthed = false;
}

// Called when the game starts or when spawned
void ARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//HPComp->OnHealthChanged.AddDynamic(this, &ARPGPlayerCharacter::Death);
}

// Called every frame
void ARPGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForInteractableObjects(TraceLength, false);
}

// Called to bind functionality to input
void ARPGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup action bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARPGPlayerCharacter::StartJumping);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARPGPlayerCharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARPGPlayerCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARPGPlayerCharacter::StopSprinting);
	PlayerInputComponent->BindAction("SwitchPOV", IE_Pressed, this, &ARPGPlayerCharacter::SwitchPOV);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARPGPlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ARPGPlayerCharacter::ToggleInventory);
	PlayerInputComponent->BindAction("StealthToggle", IE_Pressed, this, &ARPGPlayerCharacter::OnStealthPressed);

	// Setup axis bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPGPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPGPlayerCharacter::LookUpAtRate);
}

void ARPGPlayerCharacter::MoveForward(float Value)
{
	if (!Controller)
	{	
		// Controller is invalid
		return;
	}
	
	if (Value == FLT_EPSILON)
	{
		// Value is nearly zero, do nothing
		return;
	}

	// Define rotation to get forward vector from
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ARPGPlayerCharacter::MoveRight(float Value)
{
	if (!Controller)
	{
		// Controller is invalid
		return;
	}

	if (Value == FLT_EPSILON)
	{
		// Value is nearly zero, do nothing
		return;
	}

	// Find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ARPGPlayerCharacter::StartJumping()
{
	ACharacter::Jump();

	// MyTODO: Make character stop rotating while in air
}

void ARPGPlayerCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void ARPGPlayerCharacter::StartSprinting()
{
	if (bStealthed)
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = 4500.f;
}

void ARPGPlayerCharacter::StopSprinting()
{
	if (bStealthed)
	{
		return;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ARPGPlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayerCharacter::Death()
{
	if (!HPComp)
	{
		// HPComp is invalid
		return;
	}

	if (HPComp->bDiedAlready)
	{
		// We already died
		return;
	}

	HPComp->SetCurrentHealth(0.f);
	HPComp->bDiedAlready = true;

	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Disable input
	DisableInput(Cast<APlayerController>(GetController()));

	// Call blueprint event
	OnDied();

	/* Cosmetics */
	// AnimMontage
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}

	// SFX
	if (AkComp)
	{
		AkComp->PostAkEvent(CharacterSoundCollection->DeathSound, 0, nullptr, nullptr);
	}
}

void ARPGPlayerCharacter::CalculateFallDamage()
{
	if (!bCanGetDamagedFromFalling)
	{
		// Can't receive fall damage
		return;
	}
	
	const float InvertedZVelocity = GetCharacterMovement()->Velocity.Z * -1.f;
	
	if (InvertedZVelocity < FallDamageMinimalThreshold)
	{
		// Not enough velocity to take damage
		return;
	}

	const float CharacterMaxHealthPercent = HPComp->GetMaxHealth() / 100.f;
	const float ActualDamage = FMath::RoundToFloat(InvertedZVelocity / 100.f * FallDamageMultiplier * CharacterMaxHealthPercent);

	UGameplayStatics::ApplyDamage(this, ActualDamage, nullptr, nullptr, UDamageTypeEnviromental::StaticClass());

	DEBUGMESSAGE(3.f, FColor::Red, "Damage taken from falling: %f", ActualDamage);
}

void ARPGPlayerCharacter::Landed(const FHitResult& Hit)
{
	CalculateFallDamage();
}

void ARPGPlayerCharacter::SwitchPOV()
{
	switch (PlayerPOV)
	{
	case EPlayerPOV::FirstPerson:
	
		SpringArmComp->TargetArmLength = MaxTargetBoomLength;
		SpringArmComp->SocketOffset = FVector(0.f, 80.f, 0.f);

		GetMesh()->SetVisibility(true);

		TraceLength = 700.f;

		PlayerPOV = EPlayerPOV::ThirdPerson;
		
		OnPOVChanged();
		break;

	case EPlayerPOV::ThirdPerson:

		SpringArmComp->TargetArmLength = MinTargetBoomLength;
		SpringArmComp->SocketOffset = FVector(0.f, 0.f, 0.f);

		GetMesh()->SetVisibility(false);

		PlayerPOV = EPlayerPOV::FirstPerson;

		TraceLength = 350.f;

		OnPOVChanged();
		break;
		
	default:
		break;
	}
}

AActor* ARPGPlayerCharacter::TraceForInteractableObjects(float inTraceLength, bool bDrawDebugLine)
{
	if (bInDialog)
	{
		// Don't trace if we are in dialog
		return nullptr;
	}

	const FVector StartLoc = CameraComp->GetComponentLocation();
	const FVector EndLoc = (StartLoc + (CameraComp->GetForwardVector() * inTraceLength)); // Start location of linetrace is added to camera's forward vector which is multiplied by Trace length

	FHitResult HitResult;

	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	const bool bHitResult = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_WorldDynamic, CQP);

	if (bHitResult)
	{
		// Draw debug line if set to
		if (bDrawDebugLine)
		{
			DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 4.f, 0, 2.f);
		}

		AActor* HitActor = HitResult.GetActor();

		// Show interaction prompt if actor is eligible
		if (HitActor->GetClass()->ImplementsInterface(URPGInteract_Interface::StaticClass()))
		{
			if (MainHUD_WidgetRef)
			{
				MainHUD_WidgetRef->DisplayInteractionMessage(true, IRPGInteract_Interface::Execute_GetName(HitActor));
			}
 		}
		// Delete interaction prompt if HitActior doesn't implement the interface (providing it exists)
		else
		{
			if (MainHUD_WidgetRef)
			{
				MainHUD_WidgetRef->DisplayInteractionMessage(false, FText::FromString(""));
			}
		}

		return InteractActor = HitActor;
	}

	// Delete interaction prompt if no HitActor present
	if (MainHUD_WidgetRef)
	{
		MainHUD_WidgetRef->DisplayInteractionMessage(false, FText::FromString(""));
	}

	return InteractActor = nullptr;
}

void ARPGPlayerCharacter::Interact()
{
	if (!InteractActor)
	{
		// Interact actor doesn't exist
		return;
	}

	if (!InteractActor->GetClass()->ImplementsInterface(URPGInteract_Interface::StaticClass()))
	{
		// Interact actor doesn't implement the interface
		return;
	}

	auto* InteractActorCasted = Cast<IRPGInteract_Interface>(InteractActor);
	if (!InteractActorCasted)
	{
		// Interact actor doesn't implement the interface
		return;
	}

	// Interact with the actor 
	//IRPGInteract_Interface::Execute_Interact(InteractActor, this);
	InteractActorCasted->InteractNative(this);
}

void ARPGPlayerCharacter::ToggleInventory()
{
	InventoryComp->ToggleInventory();
}

void ARPGPlayerCharacter::RPGChangeCharacterModel()
{
	int DataPieceIndex = 255;

	// Find our current mesh in the collection
	for (int i = 0; i < CharacterSelectionData.Num(); i++)
	{
		const auto& DataPiece = CharacterSelectionData[i];

		if (DataPiece.SkeletalMesh == GetMesh()->GetSkeletalMeshAsset())
		{
			DataPieceIndex = i;
			break;
		}
	}

	// Get data from next index if valid
	const auto& NextDataPiece = CharacterSelectionData.IsValidIndex(DataPieceIndex + 1) ? CharacterSelectionData[DataPieceIndex + 1] : CharacterSelectionData[0];
	auto* NextSKM = NextDataPiece.SkeletalMesh;
	const TSubclassOf<UAnimInstance> NextAnimBP = NextDataPiece.AssociatedAnimBP;

	// Apply to our character
	if (NextSKM && NextAnimBP)
	{
		GetMesh()->SetSkeletalMesh(NextSKM);
		GetMesh()->SetAnimInstanceClass(NextAnimBP);
	}
}

void ARPGPlayerCharacter::OnStealthPressed()
{
	if (bStealthed)
	{
		// to default state
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		// to stealthed state
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
	}

	bStealthed = !bStealthed;

	BlueprintOnStealthPressed();

	UE_LOG(LogRPGPlayerCharacter, Warning, TEXT("Stealth mode changed. %s now."), bStealthed ? TEXT("Active") : TEXT("Disabled"));
}