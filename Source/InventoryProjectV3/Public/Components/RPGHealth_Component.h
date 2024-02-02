// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGHealth_Component.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, URPGHealth_Component*, HealthComp, float, Health, float, HealthDelta,
const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYPROJECTV3_API URPGHealth_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGHealth_Component();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bDiedAlready;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddCurrentHealth(float HealthToAdd);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddMaxHealth(float HealthToAdd);

	UPROPERTY(BlueprintCallable, Category = "Health")
	FOnHealthChangedSignature OnHealthChanged;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

public:	

	virtual void OnRegister() override;

#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
