// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGAbilityPickup.h"

#include "Components/InterpToMovementComponent.h"
#include "Components/SphereComponent.h"

ARPGAbilityPickup::ARPGAbilityPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	InterpToMovementComponent = CreateDefaultSubobject<UInterpToMovementComponent>(TEXT("InterpToMovementComponent"));
	InterpToMovementComponent->BehaviourType = EInterpToBehaviourType::PingPong;
	
	const FInterpControlPoint ControlPointA;
	const FInterpControlPoint ControlPointB(FVector(0.f, 0.f, 20.f), true);
	
	InterpToMovementComponent->ControlPoints.Add(ControlPointA);
	InterpToMovementComponent->ControlPoints.Add(ControlPointB);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	
	AbilityIconMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AbilityIconMeshComponent"));
}

void ARPGAbilityPickup::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARPGAbilityPickup::OnSphereComponentBeginOverlap);
}

void ARPGAbilityPickup::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
}
