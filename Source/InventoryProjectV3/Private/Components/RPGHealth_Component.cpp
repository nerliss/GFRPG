// Destruction Games. 2022


#include "Components/RPGHealth_Component.h"
#include "Utility//Utility.h"
#include "Characters/RPGPlayerCharacter.h"

// Sets default values for this component's properties
URPGHealth_Component::URPGHealth_Component()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	bDiedAlready = false;
}

// Called when the game starts
void URPGHealth_Component::BeginPlay()
{
	Super::BeginPlay();
}

void URPGHealth_Component::OnRegister()
{
	Super::OnRegister();
	
	// Bind TakeDamage function to OnTakeAnyDamage event for owner 
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddUniqueDynamic(this, &URPGHealth_Component::TakeDamage);
	}
}

#if WITH_EDITORONLY_DATA
void URPGHealth_Component::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	// If MaxHealth is changed, set CurrentHealth to MaxHealth
	CurrentHealth = MaxHealth;

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Called every frame
void URPGHealth_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGHealth_Component::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// If damage is invalid - ignore
	if (Damage <= 0 || bDiedAlready)
	{
		return;
	}

	// Handle take damage
	const float NewHealth = GetCurrentHealth() - Damage;
	const float ClampedNewHealth = FMath::Clamp(NewHealth, 0.f, GetMaxHealth());

	SetCurrentHealth(ClampedNewHealth);

	// MyTODO: Either bind some functionality to this delegate or remove it, right now nothing is bound
	OnHealthChanged.Broadcast(this, ClampedNewHealth, Damage, DamageType, InstigatedBy, DamageCauser);

	// MyTODO: Move death function to this component, redesign if needed
	auto* Owner = Cast<ARPGPlayerCharacter>(GetOwner());
	if (Owner)
	{
		if (GetCurrentHealth() <= 0)
		{
			Owner->Death();
		}
	}
}

float URPGHealth_Component::GetCurrentHealth()
{
	return CurrentHealth;
}

void URPGHealth_Component::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = NewHealth;
}

void URPGHealth_Component::AddCurrentHealth(float HealthToAdd)
{
	CurrentHealth += HealthToAdd;
}

float URPGHealth_Component::GetMaxHealth()
{
	return MaxHealth;
}

void URPGHealth_Component::SetMaxHealth(float NewHealth)
{
	MaxHealth = NewHealth;
}

void URPGHealth_Component::AddMaxHealth(float HealthToAdd)
{
	MaxHealth += HealthToAdd;
}
