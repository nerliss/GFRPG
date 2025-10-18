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
		
}

void ARPG3DWorldMarker::UpdateOwner()
{
	
}

void ARPG3DWorldMarker::Initialize()
{
	
}
