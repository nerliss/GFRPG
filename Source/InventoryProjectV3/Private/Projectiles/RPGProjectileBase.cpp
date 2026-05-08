// Oleksandr Tkachov 2021-2026


#include "Projectiles/RPGProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

ARPGProjectileBase::ARPGProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void ARPGProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}
