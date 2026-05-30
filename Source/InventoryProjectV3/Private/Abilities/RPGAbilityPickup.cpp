// Oleksandr Tkachov 2021-2026


#include "Abilities/RPGAbilityPickup.h"

#include "Abilities/RPGAbilityComponent.h"
#include "Abilities/RPGAbilityDefinitionData.h"
#include "Characters/RPGCharacter.h"
#include "Characters/RPGPlayerCharacter.h"
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
	SetRootComponent(SphereComponent);
	
	AbilityIconMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AbilityIconMeshComponent"));
	AbilityIconMeshComponent->AttachToComponent(SphereComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	
	AbilityDefinition = nullptr;
	AbilityIconMeshMaterial = nullptr;
}

void ARPGAbilityPickup::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARPGAbilityPickup::OnSphereComponentBeginOverlap);
	
	CreateAbilityIconMeshDynamicMaterial();
}

#if WITH_EDITORONLY_DATA
void ARPGAbilityPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	CreateAbilityIconMeshDynamicMaterial();
}
#endif

void ARPGAbilityPickup::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                      bool bFromSweep, const FHitResult& SweepResult)
{
	if (!AbilityDefinition)
	{
		return;
	}
	
	if (!OtherActor)
	{
		return;
	}
	
	const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(OtherActor);
	if (!PlayerCharacter)
	{
		return;
	}
	
	URPGAbilityComponent* AbilityComp = PlayerCharacter->GetAbilityComponent();
	if (!AbilityComp)
	{
		return;
	}
	
	AbilityComp->AddAbility(AbilityDefinition);
	
	Destroy();
}

void ARPGAbilityPickup::CreateAbilityIconMeshDynamicMaterial()
{
	if (!AbilityIconMeshComponent)
	{
		return;
	}
	
	if (!AbilityDefinition)
	{
		return;
	}
	
	if (!AbilityDefinition->Icon)
	{
		return;
	}
	
	if (!AbilityIconMeshMaterial)
	{
		return;
	}
	
	UMaterialInstanceDynamic* DynamicMaterial = AbilityIconMeshComponent->CreateDynamicMaterialInstance(0, AbilityIconMeshMaterial);
	if (!DynamicMaterial)
	{
		return;
	}
	
	DynamicMaterial->SetTextureParameterValue(FName("AbilityIcon"), AbilityDefinition->Icon);
}
