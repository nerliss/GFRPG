// Oleksandr Tkachov 2021-2026


#include "Components/RPGHealth_Component.h"
#include "Characters/RPGPlayerCharacter.h"
#include "DamageTypes/DamageTypeEnviromental.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

URPGHealth_Component::URPGHealth_Component()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	bDiedAlready = false;
}

void URPGHealth_Component::OnRegister()
{
	Super::OnRegister();
	
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

void URPGHealth_Component::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
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

void URPGHealth_Component::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = NewHealth;
}

void URPGHealth_Component::ModifyCurrentHealth(float HealthToAdd)
{
	if (HealthToAdd < 0)
	{
		// TODO: test this out - may be need to pass -HealthToAdd (since ApplyDamage won't take a negative number)
		UGameplayStatics::ApplyDamage(GetOwner(), +HealthToAdd, GetOwner()->GetInstigatorController(), GetOwner(), UDamageTypeEnviromental::StaticClass());
		return;
	}
	
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthToAdd, 0.0f, MaxHealth);
	
	// Trigger the delegate when health is gained too
	const UDamageType* const DamageType = UDamageTypeEnviromental::StaticClass()->GetDefaultObject<UDamageType>();	
	OnHealthChanged.Broadcast(this, CurrentHealth, HealthToAdd, DamageType, GetOwner()->GetInstigatorController(), GetOwner());
}

void URPGHealth_Component::SetMaxHealth(float NewHealth)
{
	MaxHealth = NewHealth;
}

void URPGHealth_Component::ModifyMaxHealth(float HealthToAdd)
{
	MaxHealth += HealthToAdd;
}
