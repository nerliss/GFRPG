// Oleksandr Tkachov 2022-2023


#include "PlayerController/RPGPlayer_Controller.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/RPGHUD_Widget.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Characters/PhotoModePawn.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(LogRPGPlayerController);

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

	// Check if MainHUD_WidgetClass is valid
	if (MainHUDWidgetClass)
	{
		// Create widget derived from URPGHUD_Widget
		MainHUDWidget = Cast<URPGHUD_Widget>(CreateWidget(GetWorld(), MainHUDWidgetClass));

		if (MainHUDWidget)
		{
			// Set player reference
			ARPGPlayerCharacter* PlayerRef = Cast<ARPGPlayerCharacter>(GetPawn());

			// Set HUD widget reference in Player character
			if (PlayerRef)
			{
				PlayerRef->MainHUD_WidgetRef = MainHUDWidget;

				MainHUDWidget->AddToViewport();
				
				UE_LOG(LogRPGPlayerController, Log, TEXT("[RPGPlayer_Controller::BeginPlay] MainHUD reference is set"));
			}
		}
	}

	// Save reference to the original player pawn
	OriginalPlayerPawn = GetPawn();
}

void ARPGPlayer_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("TogglePhotoMode", IE_Pressed, this, &ARPGPlayer_Controller::TogglePhotoMode);
}

void ARPGPlayer_Controller::SpawnPhotoModePawn()
{
	if (!PhotoModePawnClass)
	{
		UE_LOG(LogRPGPlayerController, Error, TEXT("[RPGPlayer_Controller::SpawnPhotoModePawn] Couldn't spawn a photo mode pawn: the class to spawn from is unset!"));
		return;
	}

	const FTransform SpawnTransform = OriginalPlayerPawn->GetActorTransform();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PhotoModePawn = GetWorld()->SpawnActor<APhotoModePawn>(PhotoModePawnClass, SpawnTransform, SpawnParams);

	// Unpossess the original pawn and possess the photo mode pawn
	UnPossess();
	Possess(Cast<APawn>(PhotoModePawn));

	UE_LOG(LogRPGPlayerController, Log, TEXT("[RPGPlayer_Controller::SpawnPhotoModePawn] Photo mode pawn spawned and possessed!"));
}

void ARPGPlayer_Controller::DespawnPhotoModePawn()
{
	// Unpossess photo mode pawn and possess the original pawn
	UnPossess();
	Possess(OriginalPlayerPawn);

	GetWorld()->DestroyActor(PhotoModePawn);
	PhotoModePawn = nullptr;

	UE_LOG(LogRPGPlayerController, Log, TEXT("[RPGPlayer_Controller::DespawnPhotoModePawn] Photo mode pawn despawned and original pawn possessed!"));
}

void ARPGPlayer_Controller::TogglePhotoMode()
{
	PhotoModePawn ? DespawnPhotoModePawn() : SpawnPhotoModePawn();
}
