// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGAbilityComponent.generated.h"


class URPGAbilityBase;
class URPGAbilityDefinitionData;

USTRUCT(BlueprintType)
struct FRPGTargetData
{
	GENERATED_BODY()

	// TODO: Probably doesn't need to be EditDefaultsOnly
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	TObjectPtr<AActor> HitActor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	FVector HitLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	FVector AimDirection;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
	bool bHasHit;
	
	UPROPERTY(BlueprintReadWrite)
	FHitResult HitResult;
	
	FRPGTargetData()
	{
		HitActor = nullptr;
		HitLocation = FVector::ZeroVector;
		AimDirection = FVector::ForwardVector;
		bHasHit = false;
		HitResult = FHitResult();
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityCooldownStarted, URPGAbilityBase*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityCooldownEnded, URPGAbilityBase*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityAdded, URPGAbilityDefinitionData*, AbilityDefinition);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URPGAbilityComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "Trace")
	void TraceForTargetData(const float InTraceLength, URPGAbilityBase* Ability, FRPGTargetData& OutTargetData);
	
	UFUNCTION(BlueprintCallable)
	void SetTimerForAbilityCooldownExpiration(URPGAbilityBase* Ability);	
	
	UFUNCTION()
	void OnCooldownTimerExpired(URPGAbilityBase* Ability);
	
	UFUNCTION(BlueprintCallable)
	FTimerHandle SetTimerForCastAbility(URPGAbilityBase* Ability, FRPGTargetData TargetData);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnCastFinished(URPGAbilityBase* Ability, FRPGTargetData TargetData);
	
	// TODO: Make pure if these functions need to be exposed at all
	UFUNCTION(BlueprintCallable)
	float GetCooldownRemainingForAbility(URPGAbilityBase* Ability) const;
	
	UFUNCTION(BlueprintCallable)
	float GetCooldownPercentForAbility(URPGAbilityBase* Ability) const;
	
	UFUNCTION(BlueprintCallable)
	float GetCooldownDurationForAbility(URPGAbilityBase* Ability) const;
	
	UFUNCTION(BlueprintCallable)
	bool IsAbilityOnCooldown(URPGAbilityBase* Ability) const;
	
	UFUNCTION(BlueprintCallable)
	void AddAbility(URPGAbilityDefinitionData* NewAbilityDefinition);
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityCooldownStarted OnAbilityCooldownStarted;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityCooldownEnded OnAbilityCooldownEnded;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityAdded OnAbilityAdded;
	
	// TODO: Change property params later on to be more read only
	/* Abilities to spawn from. Literally a set of abilities that will be spawned upon starting a game for specific component*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<URPGAbilityDefinitionData*> TemplateAbilityDefinitions;
	
	/* Spawned abilities for this component, mapped to a spawned object */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TMap<URPGAbilityDefinitionData*, URPGAbilityBase*> SpawnedAbilityDefinitions;
	
};
