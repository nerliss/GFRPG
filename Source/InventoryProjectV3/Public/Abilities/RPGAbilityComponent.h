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
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityCooldownStarted OnAbilityCooldownStarted;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityCooldownEnded OnAbilityCooldownEnded;
	
};
