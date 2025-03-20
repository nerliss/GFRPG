// Oleksandr Tkachov 2022-2025


#include "PlayerController/RPGPlayer_Controller.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/RPGHUD_Widget.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Characters/PhotoModePawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Utility/LogDefinitions.h"

ARPGPlayer_Controller::ARPGPlayer_Controller()
{
	// Widgets
	// Find the HUD widget class
	//static ConstructorHelpers::FClassFinder<UUserWidget> MainHUDObject(TEXT("/Game/InventoryProject/Core/UI/WBP_HUD_Cpp"));
	//MainHUDWidgetClass = MainHUDObject.Class;
	MainHUDWidget = nullptr;

	// Photo mode
	OriginalPlayerPawn = nullptr;
	PhotoModePawn = nullptr;
}

void ARPGPlayer_Controller::BeginPlay()
{
	Super::BeginPlay();

	InitializeHUDWidget();

	// Save reference to the original player pawn
	OriginalPlayerPawn = GetPawn();
}

void ARPGPlayer_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("TogglePhotoMode", IE_Pressed, this, &ARPGPlayer_Controller::TogglePhotoMode);
}

void ARPGPlayer_Controller::InitializeHUDWidget()
{
	if (!MainHUDWidgetClass)
	{
		UE_LOG(LogRPGUIHUD, Error, TEXT("[ARPGPlayer_Controller::InitializeHUDWidget] Couldn't create a HUD widget: the class to spawn from is unset!"));
		return;
	}

	MainHUDWidget = Cast<URPGHUD_Widget>(CreateWidget(GetWorld(), MainHUDWidgetClass));
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

	UE_LOG(LogRPGUIHUD, Verbose, TEXT("[ARPGPlayer_Controller::InitializeHUDWidget] HUD Widget successfuly initialized!"));
}

URPGHUD_Widget* ARPGPlayer_Controller::GetHUDWidget() const
{
	return MainHUDWidget;
}

void ARPGPlayer_Controller::SpawnPhotoModePawn()
{
	if (!PhotoModePawnClass)
	{
		UE_LOG(LogRPGPhotoMode, Error, TEXT("[ARPGPlayer_Controller::SpawnPhotoModePawn] Couldn't spawn a photo mode pawn: the class to spawn from is unset!"));
		return;
	}

	const FTransform SpawnTransform = OriginalPlayerPawn->GetActorTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PhotoModePawn = GetWorld()->SpawnActor<APhotoModePawn>(PhotoModePawnClass, SpawnTransform, SpawnParams);

	// Unpossess the original pawn and possess the photo mode pawn
	UnPossess();
	Possess(Cast<APawn>(PhotoModePawn));

	UE_LOG(LogRPGPhotoMode, Verbose, TEXT("[ARPGPlayer_Controller::SpawnPhotoModePawn] Photo mode pawn spawned and possessed!"));
}

void ARPGPlayer_Controller::DespawnPhotoModePawn()
{
	// Unpossess photo mode pawn and possess the original pawn
	UnPossess();
	Possess(OriginalPlayerPawn);

	GetWorld()->DestroyActor(PhotoModePawn);
	PhotoModePawn = nullptr;

	UE_LOG(LogRPGPhotoMode, Verbose, TEXT("[ARPGPlayer_Controller::DespawnPhotoModePawn] Photo mode pawn despawned and original pawn possessed!"));
}

void ARPGPlayer_Controller::TogglePhotoMode()
{
	PhotoModePawn ? DespawnPhotoModePawn() : SpawnPhotoModePawn();
}
