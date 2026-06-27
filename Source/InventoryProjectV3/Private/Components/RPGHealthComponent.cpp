// Oleksandr Tkachov 2021-2026


#include "Components/RPGHealthComponent.h"
#include "Characters/RPGPlayerCharacter.h"
#include "DamageTypes/DamageTypeEnviromental.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

URPGHealthComponent::URPGHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	bDiedAlready = false;
}

void URPGHealthComponent::OnRegister()
{
	Super::OnRegister();
	
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddUniqueDynamic(this, &URPGHealthComponent::TakeDamage);
	}
}

#if WITH_EDITORONLY_DATA
void URPGHealthComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	// If MaxHealth is changed, set CurrentHealth to MaxHealth
	CurrentHealth = MaxHealth;

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void URPGHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0 || bDiedAlready)
	{
		return;
	}

	// Handle take damage
	const float NewHealth = GetCurrentHealth() - Damage;
	const float ClampedNewHealth = FMath::Clamp(NewHealth, 0.f, GetMaxHealth());

	SetCurrentHealth(ClampedNewHealth);

	// TODO: Either bind some functionality to this delegate or remove it, right now nothing is bound
	OnHealthChanged.Broadcast(this, ClampedNewHealth, Damage, DamageType, InstigatedBy, DamageCauser);

	// TODO: Move death function to this component, redesign if needed
	ARPGPlayerCharacter* Owner = Cast<ARPGPlayerCharacter>(GetOwner());
	if (Owner)
	{
		if (GetCurrentHealth() <= 0)
		{
			Owner->Death();
		}
	}
}

void URPGHealthComponent::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = NewHealth;
}

void URPGHealthComponent::ModifyCurrentHealth(float HealthToAdd)
{
	if (HealthToAdd < 0)
	{
		UGameplayStatics::ApplyDamage(GetOwner(), -HealthToAdd, GetOwner()->GetInstigatorController(), GetOwner(), UDamageTypeEnviromental::StaticClass());
		return;
	}
	
	const float ClampedNewHealth = FMath::Clamp(CurrentHealth + HealthToAdd, 0.0f, MaxHealth);
	
	SetCurrentHealth(ClampedNewHealth);
	
	// Trigger the delegate when health is gained too
	const UDamageType* const DamageType = UDamageTypeEnviromental::StaticClass()->GetDefaultObject<UDamageType>();	
	OnHealthChanged.Broadcast(this, CurrentHealth, HealthToAdd, DamageType, GetOwner()->GetInstigatorController(), GetOwner());
}

void URPGHealthComponent::SetMaxHealth(float NewHealth)
{
	MaxHealth = NewHealth;
}

void URPGHealthComponent::ModifyMaxHealth(float HealthToAdd)
{
	MaxHealth += HealthToAdd;
}
