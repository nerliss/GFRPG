// Oleksandr Tkachov 2022-2023


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
#include "Utility/LogDefinitions.h"
#include "Interfaces/RPGInteract_Interface.h"
#include "DamageTypes/DamageTypeEnviromental.h"
#include "DataAssets/CharacterSoundCollection.h"
#include "GameInstance/RPGGameInstanceBase.h"
#include "Save/RPGSaveGameObject.h"

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CvarSuperSprint(TEXT("DebugSuperSprint"), 0, TEXT("Enable to use super sprint speed instead of default one"));
static TAutoConsoleVariable<int32> CvarSuperJump(TEXT("DebugSuperJump"), 0, TEXT("Enable to use super jump. Gives full free controll of the pawn in air, double jump and force enables DebugIgnoreFallDamage."));
static TAutoConsoleVariable<int32> CvarIgnoreFallDamage(TEXT("DebugIgnoreFallDamage"), 0, TEXT("Enable to ignore fall damage"));
#endif

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

	DefaultMaxWalkSpeed = 600.f;
	SprintMaxWalkSpeed = 1000.f;
	StealthedMaxWalkSpeed = 250.f;
	bStealthed = false;
}

void ARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	LoadLastCharacterModel();

	// MyTODO: Collapse to a function when more related variables are added
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void ARPGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForInteractableObjects(TraceLength, false);
}

void ARPGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARPGPlayerCharacter::OnJumpStarted);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARPGPlayerCharacter::OnJumpEnded);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARPGPlayerCharacter::OnSprintStarted);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARPGPlayerCharacter::OnSprintStopped);
	PlayerInputComponent->BindAction("SwitchPOV", IE_Pressed, this, &ARPGPlayerCharacter::OnPOVSwitched);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARPGPlayerCharacter::OnInteractPressed);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ARPGPlayerCharacter::OnInventoryToggled);
	PlayerInputComponent->BindAction("StealthToggle", IE_Pressed, this, &ARPGPlayerCharacter::OnStealthPressed);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGPlayerCharacter::OnForwardMoved);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGPlayerCharacter::OnRightMoved);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPGPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPGPlayerCharacter::LookUpAtRate);
}

#if WITH_EDITORONLY_DATA
void ARPGPlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	LoadLastCharacterModelInternal();
}
#endif

void ARPGPlayerCharacter::OnForwardMoved(float Value)
{
	if (FMath::IsNearlyZero(Value))
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

void ARPGPlayerCharacter::OnRightMoved(float Value)
{
	if (FMath::IsNearlyZero(Value))
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

void ARPGPlayerCharacter::OnJumpStarted()
{
#if !UE_BUILD_SHIPPING
	if (CvarSuperJump.GetValueOnGameThread() > 0)
	{
		GetCharacterMovement()->JumpZVelocity = 3000.f; 
		GetCharacterMovement()->AirControl = 1.f;
		JumpMaxCount = 2;
		JumpMaxHoldTime = 0.3f;

		CvarIgnoreFallDamage.AsVariable()->Set(1, ECVF_SetByConsole);
	}
#endif

	ACharacter::Jump();
}

void ARPGPlayerCharacter::OnJumpEnded()
{
#if !UE_BUILD_SHIPPING
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	JumpMaxCount = 1;
	JumpMaxHoldTime = 0.f;
#endif

	ACharacter::StopJumping();
}

void ARPGPlayerCharacter::OnSprintStarted()
{
	if (bStealthed)
	{
		return;
	}

#if !UE_BUILD_SHIPPING
	if (CvarSuperSprint.GetValueOnGameThread() > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 5500.f;
		return;
	}
#endif

	GetCharacterMovement()->MaxWalkSpeed = SprintMaxWalkSpeed;
}

void ARPGPlayerCharacter::OnSprintStopped()
{
	if (bStealthed)
	{
		return;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void ARPGPlayerCharacter::OnStealthPressed()
{
	const float NewMaxWalkSpeed = bStealthed ? DefaultMaxWalkSpeed : StealthedMaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;

	bStealthed = !bStealthed;

	BlueprintOnStealthPressed();

	UE_LOG(LogRPGPlayerCharacter, Warning, TEXT("Stealth mode changed. %s now."), bStealthed ? TEXT("Active") : TEXT("Disabled"));
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
		return;
	}

	if (HPComp->bDiedAlready)
	{
		// We already died
		return;
	}

	HPComp->SetCurrentHealth(0.f);
	HPComp->bDiedAlready = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
#if !UE_BUILD_SHIPPING
	if (CvarIgnoreFallDamage.GetValueOnGameThread() > 0)
	{
		return;
	}
#endif

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

void ARPGPlayerCharacter::OnPOVSwitched()
{
	switch (PlayerPOV)
	{
	case EPlayerPOV::FirstPerson:
	
		SpringArmComp->TargetArmLength = MaxTargetBoomLength;
		SpringArmComp->SocketOffset = FVector(0.f, 80.f, 0.f);

		GetMesh()->SetVisibility(true);

		TraceLength = 700.f;

		PlayerPOV = EPlayerPOV::ThirdPerson;
		
		break;

	case EPlayerPOV::ThirdPerson:

		SpringArmComp->TargetArmLength = MinTargetBoomLength;
		SpringArmComp->SocketOffset = FVector(0.f, 0.f, 0.f);

		GetMesh()->SetVisibility(false);

		PlayerPOV = EPlayerPOV::FirstPerson;

		TraceLength = 350.f;

		break;
		
	default:
		break;
	}

	OnPOVChangedBlueprint();
}

AActor* ARPGPlayerCharacter::TraceForInteractableObjects(float inTraceLength, bool bDrawDebugLine)
{
	if (bInDialog)
	{
		// Don't trace if we are in dialog
		return nullptr;
	}

	if (!MainHUD_WidgetRef)
	{
		return nullptr;
	}

	const FVector StartLoc = CameraComp->GetComponentLocation();
	const FVector EndLoc = (StartLoc + (CameraComp->GetForwardVector() * inTraceLength));

	FHitResult HitResult;

	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	const bool bHitResult = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_WorldDynamic, CQP);

	if (!bHitResult)
	{
		MainHUD_WidgetRef->DisplayInteractionMessage(false, FText::FromString(""));
		return InteractActor = nullptr;
	}

	// Draw debug line if set to
	if (bDrawDebugLine)
	{
		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 4.f, 0, 2.f);
	}

	auto* HitActor = HitResult.GetActor();

	auto* InteractActorCasted = Cast<IRPGInteract_Interface>(HitActor);
	if (!InteractActorCasted)
	{
		MainHUD_WidgetRef->DisplayInteractionMessage(false, FText::FromString(""));
		return InteractActor = nullptr;;
	}

	// Show interaction prompt if actor is eligible
	if (HitActor->GetClass()->ImplementsInterface(URPGInteract_Interface::StaticClass()))
	{
		// MyTODO: Figure out a way to use one function that can be overriden both in C++ and BP
		MainHUD_WidgetRef->DisplayInteractionMessage(true, IRPGInteract_Interface::Execute_GetName(HitActor));
		MainHUD_WidgetRef->DisplayInteractionMessage(true, InteractActorCasted->GetNameNative());
		return InteractActor = HitActor;
	}

	MainHUD_WidgetRef->DisplayInteractionMessage(false, FText::FromString(""));
	return InteractActor = nullptr;
}

void ARPGPlayerCharacter::OnInteractPressed()
{
	if (!InteractActor)
	{
		return;
	}

	if (!InteractActor->GetClass()->ImplementsInterface(URPGInteract_Interface::StaticClass()))
	{
		// Actor is not interactable
		return;
	}

	auto* InteractActorCasted = Cast<IRPGInteract_Interface>(InteractActor);
	if (!InteractActorCasted)
	{
		// Interact actor doesn't implement the interface
		return;
	}

	InteractActorCasted->InteractNative(this);
	UE_LOG(LogRPGPlayerCharacter, Log, TEXT("Interacting with %s"), *InteractActor->GetName());
}

void ARPGPlayerCharacter::OnInventoryToggled()
{
	InventoryComp->ToggleInventory();
}

void ARPGPlayerCharacter::LoadLastCharacterModel()
{
	const auto* RPGGameInstance = GetRPGGameInstance();
	check(RPGGameInstance);

	const auto SaveCharacterData = RPGGameInstance->GetSaveGameObject()->CharacterPlayerData;
	if (!(SaveCharacterData.SkeletalMesh && SaveCharacterData.AssociatedAnimBP))
	{	
		UE_LOG(LogRPGPlayerCharacter, Error, TEXT("[ARPGPlayerCharacter::LoadLastCharacterModel] SaveCharacterData doesn't have SkeletalMesh and/or AssociatedAnimBP!"));
		return;
	}

	GetMesh()->SetSkeletalMeshAsset(SaveCharacterData.SkeletalMesh);
	GetMesh()->SetAnimInstanceClass(SaveCharacterData.AssociatedAnimBP);

	UE_LOG(LogRPGPlayerCharacter, Log, TEXT("[ARPGPlayerCharacter::LoadLastCharacterModel] Last character model was loaded"));
}

#if WITH_EDITORONLY_DATA
void ARPGPlayerCharacter::LoadLastCharacterModelInternal()
{
	FCharacterSelectionData LoadedData;
	TArray<uint8> LoadedBytes;
	const FString FilePath = FPaths::ProjectSavedDir() + GetLastSavedCharacterFileName();

	if (!FFileHelper::LoadFileToArray(LoadedBytes, *FilePath))
	{
		return;
	}

	FMemoryReader Reader(LoadedBytes);
	Reader << LoadedData;

	GetMesh()->SetSkeletalMeshAsset(LoadedData.SkeletalMesh);
	GetMesh()->SetAnimInstanceClass(LoadedData.AssociatedAnimBP);
}
#endif

URPGGameInstanceBase* ARPGPlayerCharacter::GetRPGGameInstance() const
{
	return GetWorld()->GetGameInstanceChecked<URPGGameInstanceBase>();
}
