// Oleksandr Tkachov 2021-2026


#include "PlayerController/RPGPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/RPGHUDWidget.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Characters/PhotoModePawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Utility/LogDefinitions.h"

ARPGPlayerController::ARPGPlayerController()
{
	//static ConstructorHelpers::FClassFinder<UUserWidget> MainHUDObject(TEXT("/Game/InventoryProject/Core/UI/WBP_HUD_Cpp"));
	//MainHUDWidgetClass = MainHUDObject.Class;
	MainHUDWidget = nullptr;

	OriginalPlayerPawn = nullptr;
	PhotoModePawn = nullptr;
}

URPGHUDWidget* ARPGPlayerController::GetHUDWidget() const
{
	return MainHUDWidget;
}

void ARPGPlayerController::SetDefaultInputMode()
{
	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	
	bShowMouseCursor = false;
}

void ARPGPlayerController::SetUIInputMode()
{
	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	
	bShowMouseCursor = true;
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeHUDWidget();

	OriginalPlayerPawn = GetPawn();
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("TogglePhotoMode", IE_Pressed, this, &ARPGPlayerController::TogglePhotoMode);
}

void ARPGPlayerController::InitializeHUDWidget()
{
	ensureMsgf(MainHUDWidgetClass, TEXT("[ARPGPlayerController::InitializeHUDWidget] Couldn't create HUD widget: MainHUDWidgetClass is null"));

	MainHUDWidget = Cast<URPGHUDWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));
	if (!MainHUDWidget)
	{
		return;
	}

	ARPGPlayerCharacter* PlayerRef = Cast<ARPGPlayerCharacter>(GetPawn());
	if (!PlayerRef)
	{
		return;
	}

	PlayerRef->SetMainHUDWidget(MainHUDWidget);

	MainHUDWidget->AddToViewport();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	UE_LOG(LogRPGUIHUD, Verbose, TEXT("[ARPGPlayerController::InitializeHUDWidget] HUD Widget successfuly initialized!"));
}

void ARPGPlayerController::SpawnPhotoModePawn()
{
	if (!PhotoModePawnClass)
	{
		UE_LOG(LogRPGPhotoMode, Error, TEXT("[ARPGPlayerController::SpawnPhotoModePawn] Couldn't spawn a photo mode pawn: the class to spawn from is unset!"));
		return;
	}

	const FTransform SpawnTransform = OriginalPlayerPawn->GetActorTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PhotoModePawn = GetWorld()->SpawnActor<APhotoModePawn>(PhotoModePawnClass, SpawnTransform, SpawnParams);

	// Unpossess the original pawn and possess the photo mode pawn
	UnPossess();
	Possess(Cast<APawn>(PhotoModePawn));

	UE_LOG(LogRPGPhotoMode, Verbose, TEXT("[ARPGPlayerController::SpawnPhotoModePawn] Photo mode pawn spawned and possessed!"));
}

void ARPGPlayerController::DespawnPhotoModePawn()
{
	// Unpossess photo mode pawn and possess the original pawn
	UnPossess();
	Possess(OriginalPlayerPawn);

	GetWorld()->DestroyActor(PhotoModePawn);
	PhotoModePawn = nullptr;

	UE_LOG(LogRPGPhotoMode, Verbose, TEXT("[ARPGPlayerController::DespawnPhotoModePawn] Photo mode pawn despawned and original pawn possessed!"));
}

void ARPGPlayerController::TogglePhotoMode()
{
	PhotoModePawn ? DespawnPhotoModePawn() : SpawnPhotoModePawn();
}
