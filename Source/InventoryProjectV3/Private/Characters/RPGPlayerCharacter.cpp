// Oleksandr Tkachov 2021-2026


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
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Abilities/RPGAbilityComponent.h"
#include "Components/RPGPointOfInterestComponent.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/RPGStatsComponent.h"
#include "Widgets/RPGHUD_Widget.h"
#include "Utility/Utility.h"
#include "Utility/LogDefinitions.h"
#include "Interfaces/RPGInteract_Interface.h"
#include "DamageTypes/DamageTypeEnviromental.h"
#include "DataAssets/CharacterSoundCollection.h"
#include "GameInstance/RPGGameInstanceBase.h"
#include "Map/RPGMapSubsystem.h"
#include "Save/RPGSaveGameObject.h"

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable CVarSuperSprint(TEXT("DebugSuperSprint"), 0, TEXT("Enable to use super sprint speed instead of default one."));
static TAutoConsoleVariable CVarSuperJump(TEXT("DebugSuperJump"), 0, TEXT("Enable to use super jump. Gives full free control of the pawn in air, double jump and force enables DebugIgnoreFallDamage."));
static TAutoConsoleVariable CVarIgnoreFallDamage(TEXT("DebugIgnoreFallDamage"), 0, TEXT("Enable to ignore fall damage."));
static TAutoConsoleVariable CVarDebugInteractLine(TEXT("DebugInteractLine"), 0, TEXT("Enable to debug interact line."));
#endif

ARPGPlayerCharacter::ARPGPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 600.f; 
	GetCharacterMovement()->AirControl = 0.2f; 

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent(), "head");
	SpringArmComp->TargetArmLength = 500.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset = FVector(0.f, 80.f, 0.f); // Give camera more Skyrim-like style
	SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 65.f));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	XPComp = CreateDefaultSubobject<URPGXP_Component>(TEXT("XPComp"));

	HPComp = CreateDefaultSubobject<URPGHealth_Component>(TEXT("HPComp"));
	GetHealthComponent()->SetMaxHealth(100.f + GetStatsComponent()->Stamina); // TODO: Review this usage
	GetHealthComponent()->SetCurrentHealth(HPComp->GetMaxHealth());

	ReputationComp = CreateDefaultSubobject<URPGReputation_Component>(TEXT("ReputationComp"));

	InventoryComp = CreateDefaultSubobject<URPGInventory_Component>(TEXT("InventoryComp"));

	// Player's icon should rotate
	PointOfInterestComponent->bRotateWithActor = true;
	
	GetStatsComponent()->Speed = 100.f;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	TargetBoomLengthLimits = FVector2D(0.f, SpringArmComp->TargetArmLength);
	PlayerPOV = EPlayerPOV::ThirdPerson;
	bInDialog = false;
	bCanGetDamagedFromFalling = true;
	FallDamageMinimalThreshold = 1300.f;
	FallDamageMultiplier = 3.f;
	CharacterSoundCollection = nullptr;
	DefaultMaxWalkSpeed = 600.f + GetStatsComponent()->Speed;
	SprintMaxWalkSpeed = 1000.f + GetStatsComponent()->Speed;
	StealthedMaxWalkSpeed = 250.f + GetStatsComponent()->Speed;
	bStealthed = false;
	bMounted = false;
	CharacterGender = ECharacterGender::Undefined;
}

void ARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	LoadLastCharacterModel();

	// TODO: Collapse to a function when more related variables are added
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;

	GetHealthComponent()->ModifyMaxHealth(GetStatsComponent()->Stamina); // TODO: Review this usage
	GetHealthComponent()->SetCurrentHealth(HPComp->GetMaxHealth());
}

void ARPGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForInteractableObjects(TraceLength);
}

void ARPGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// TODO: Move this to EnhancedInputComponent
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARPGPlayerCharacter::OnJumpStarted);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARPGPlayerCharacter::OnJumpEnded);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ARPGPlayerCharacter::OnSprintStarted);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ARPGPlayerCharacter::OnSprintStopped);
	PlayerInputComponent->BindAction("SwitchPOV", IE_Pressed, this, &ARPGPlayerCharacter::OnPOVSwitched);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARPGPlayerCharacter::OnInteractPressed);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ARPGPlayerCharacter::OnInventoryToggled);
	PlayerInputComponent->BindAction("ToggleQuestLog", IE_Pressed, this, &ARPGPlayerCharacter::OnQuestLogToggled);
	PlayerInputComponent->BindAction("StealthToggle", IE_Pressed, this, &ARPGPlayerCharacter::OnStealthPressed);
	PlayerInputComponent->BindAction("ToggleMapScreen", IE_Pressed, this, &ARPGPlayerCharacter::OnMapScreenToggled);

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

	if (bMounted)
	{
		return;
	}

	// Define rotation to get forward vector from
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
	
	OnCharacterMoved.Broadcast();
}

void ARPGPlayerCharacter::OnRightMoved(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	if (bMounted)
	{
		return;
	}

	// Find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
	
	OnCharacterMoved.Broadcast();
}

void ARPGPlayerCharacter::OnJumpStarted()
{
#if !UE_BUILD_SHIPPING
	if (CVarSuperJump.GetValueOnGameThread() > 0)
	{
		GetCharacterMovement()->JumpZVelocity = 3000.f; 
		GetCharacterMovement()->AirControl = 1.f;
		JumpMaxCount = 2;
		JumpMaxHoldTime = 0.3f;

		CVarIgnoreFallDamage.AsVariable()->Set(1, ECVF_SetByConsole);
	}
#endif

	ACharacter::Jump();
	
	OnCharacterMoved.Broadcast();
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
	if (CVarSuperSprint.GetValueOnGameThread() > 0)
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
	
	OnCharacterMoved.Broadcast();
}

void ARPGPlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayerCharacter::Death()
{
	if (!GetHealthComponent())
	{
		return;
	}

	if (GetHealthComponent()->bDiedAlready)
	{
		return;
	}

	GetHealthComponent()->SetCurrentHealth(0.f);
	GetHealthComponent()->bDiedAlready = true;

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
}

void ARPGPlayerCharacter::CalculateFallDamage()
{
#if !UE_BUILD_SHIPPING
	if (CVarIgnoreFallDamage.GetValueOnGameThread() > 0)
	{
		return;
	}
#endif

	if (!bCanGetDamagedFromFalling)
	{
		return;
	}
	
	const float InvertedZVelocity = GetCharacterMovement()->Velocity.Z * -1.f;
	
	if (InvertedZVelocity < FallDamageMinimalThreshold)
	{
		return;
	}

	const float CharacterMaxHealthPercent = HPComp->GetMaxHealth() / 100.f;
	const float ActualDamage = FMath::RoundToFloat(InvertedZVelocity / 100.f * FallDamageMultiplier * CharacterMaxHealthPercent);

	UGameplayStatics::ApplyDamage(this, ActualDamage, nullptr, nullptr, UDamageTypeEnviromental::StaticClass());

	LOG_WITH_FUNCTION_NAME(LogRPGPlayerCharacter, Log, TEXT("Damage taken from falling: %f"), ActualDamage);
}

void ARPGPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	CalculateFallDamage();
}

FText ARPGPlayerCharacter::GetNameNative() const
{
	return FText::FromString(TEXT("Player"));
}

void ARPGPlayerCharacter::OnPOVSwitched()
{
	switch (PlayerPOV)
	{
	case EPlayerPOV::FirstPerson:
	
		SpringArmComp->TargetArmLength = TargetBoomLengthLimits.Y;
		SpringArmComp->SocketOffset = FVector(0.f, 80.f, 0.f);

		GetMesh()->SetVisibility(true);

		TraceLength = 700.f;

		PlayerPOV = EPlayerPOV::ThirdPerson;
		
		break;

	case EPlayerPOV::ThirdPerson:

		SpringArmComp->TargetArmLength = TargetBoomLengthLimits.X;
		SpringArmComp->SocketOffset = FVector::Zero();

		GetMesh()->SetVisibility(false);

		PlayerPOV = EPlayerPOV::FirstPerson;

		TraceLength = 350.f;

		break;
		
	default:
		break;
	}

	OnPOVSwitchedBlueprint();
}

void ARPGPlayerCharacter::SetPOV(const EPlayerPOV DesiredPOV)
{
	if (DesiredPOV == PlayerPOV)
	{
		return;
	}

	OnPOVSwitched();
}

AActor* ARPGPlayerCharacter::TraceForInteractableObjects(const float InTraceLength)
{
	if (!ensure(GetMainHUDWidget()))
	{
		return nullptr;
	}

	if (bInDialog)
	{
		return nullptr;
	}

	const FVector StartLoc = CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * SpringArmComp->TargetArmLength;
	const FVector EndLoc = (StartLoc + (CameraComp->GetForwardVector() * InTraceLength));
	constexpr ECollisionChannel ECC_Interact = ECC_GameTraceChannel1; // Interact channel

	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHitResult = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Interact, Params);

	if (!bHitResult)
	{
		GetMainHUDWidget()->DisplayInteractionMessage(false, FText::FromString(""));
		return InteractActor = nullptr;
	}

#if !UE_BUILD_SHIPPING
	if (CVarDebugInteractLine.GetValueOnGameThread() > 0)
	{
		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 4.f, 0, 2.f);
	}
#endif

	AActor* HitActor = HitResult.GetActor();
	IRPGInteract_Interface* InteractActorCasted = Cast<IRPGInteract_Interface>(HitActor);
	if (!InteractActorCasted)
	{
		GetMainHUDWidget()->DisplayInteractionMessage(false, FText::FromString(""));
		return InteractActor = nullptr;;
	}

	/*if (HitActor->GetClass()->ImplementsInterface(URPGInteract_Interface::StaticClass()))
	{*/
		// TODO: MyTODO: Figure out a way to use one function that can be overriden both in C++ and BP
		//GetMainHUDWidget()->DisplayInteractionMessage(true, IRPGInteract_Interface::Execute_GetName(HitActor)); // Why do I even need this??
	GetMainHUDWidget()->DisplayInteractionMessage(true, InteractActorCasted->GetNameNative());

#if !UE_BUILD_SHIPPING
	if (CVarDebugInteractLine.GetValueOnGameThread() > 0)
	{
		DEBUGMESSAGE(0.f, FColor::Green, "Expected Interactable actor: %s (Class name: %s)", *InteractActorCasted->GetNameNative().ToString(), *HitActor->GetName());
	}
#endif

	return InteractActor = HitActor;
		/*}

		GetMainHUDWidget()->DisplayInteractionMessage(false, FText::FromString(""));
		return InteractActor = nullptr;*/
}

void ARPGPlayerCharacter::OnInteractPressed()
{
	if (!InteractActor)
	{
		return;
	}

	IRPGInteract_Interface* InteractActorCasted = Cast<IRPGInteract_Interface>(InteractActor);
	if (!InteractActorCasted)
	{
		return;
	}

	InteractActorCasted->InteractNative(this);
	LOG_WITH_FUNCTION_NAME(LogRPGPlayerCharacter, Verbose, TEXT("Interacting with %s"), *InteractActor->GetName());
}

void ARPGPlayerCharacter::OnInventoryToggled()
{
	GetInventoryComponent()->ToggleInventory();
}

void ARPGPlayerCharacter::OnQuestLogToggled()
{
	GetQuestLogComponent()->ToggleQuestLog();
}

void ARPGPlayerCharacter::OnMapScreenToggled()
{
	URPGMapSubsystem* MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	if (MapSubsystem)
	{
		MapSubsystem->ToggleMapScreen();
	}
}

void ARPGPlayerCharacter::LoadLastCharacterModel()
{
	const URPGGameInstanceBase* RPGGameInstance = GetRPGGameInstance();
	check(RPGGameInstance);

	const FCharacterSelectionData SaveCharacterData = RPGGameInstance->GetSaveGameObject()->CharacterPlayerData;
	if (!(SaveCharacterData.SkeletalMesh && SaveCharacterData.AssociatedAnimBP))
	{	
		LOG_WITH_FUNCTION_NAME(LogRPGPlayerCharacter, Error, TEXT("SaveCharacterData doesn't have SkeletalMesh and/or AssociatedAnimBP!"));
		return;
	}

	GetMesh()->SetSkeletalMeshAsset(SaveCharacterData.SkeletalMesh);
	GetMesh()->SetAnimInstanceClass(SaveCharacterData.AssociatedAnimBP);

	SetCharacterGender(SaveCharacterData.Gender);

	LOG_WITH_FUNCTION_NAME(LogRPGPlayerCharacter, Log, TEXT("Last character model was loaded"));
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
