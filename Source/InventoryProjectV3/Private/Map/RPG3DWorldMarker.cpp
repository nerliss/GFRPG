// Oleksandr Tkachov 2022-2025


#include "Map/RPG3DWorldMarker.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Map/RPG3DWorldMarkerWidget.h"

ARPG3DWorldMarker::ARPG3DWorldMarker()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionObjectType(ECC_WorldStatic);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComp->SetupAttachment(SphereComp);
	WidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 255.f));
	WidgetComp->SetCollisionProfileName(TEXT("UI"));
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetWidgetClass(WorldMarkerWidgetClass);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SphereComp);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	HighlightMeshComponent =  CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightMeshComponent"));
	HighlightMeshComponent->SetupAttachment(RootComponent);
	HighlightMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WaypointName = FText::FromString("Waypoint");
	bQuestWaypoint = false;
	MaxShowDistance = 100.f;
	PlayerDistance = 0.f;
}

void ARPG3DWorldMarker::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ARPG3DWorldMarker::UpdateDistance()
{
	if (!GetOwner())
	{
		UpdateOwner();
		Initialize();
	}

	PlayerDistance = FVector::Distance(GetOwner()->GetActorLocation(), WidgetComp->GetComponentLocation()) / 100.f;

	if (UIWorldMarker)
	{
		const FText DistanceText = FText::Format(FText::FromString("{0}m"), FText::AsNumber(FMath::RoundToInt(PlayerDistance)));
		UIWorldMarker->UpdateDistance(DistanceText, !bQuestWaypoint, PlayerDistance, MaxShowDistance);
	}

	// Restart the timer
	Initialize();
}

void ARPG3DWorldMarker::UpdateOwner()
{
	// TODO: Do we actually need this function? 
}

void ARPG3DWorldMarker::Initialize()
{
	GetWorld()->GetTimerManager().SetTimer(UpdateDistanceHandle, this, &ARPG3DWorldMarker::UpdateDistance, 1.f);

	UIWorldMarker = Cast<URPG3DWorldMarkerWidget>(WidgetComp->GetWidget());
	if (UIWorldMarker)
	{
		UIWorldMarker->UpdateName(WaypointName);
	}

	if (MeshComponent)
	{
		MeshComponent->SetVisibility(!bQuestWaypoint, true);
	}
}
