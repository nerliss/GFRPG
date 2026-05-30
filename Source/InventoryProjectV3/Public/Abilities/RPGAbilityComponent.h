// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "RPGAbilityDefinitionData.h"
#include "Components/ActorComponent.h"
#include "RPGAbilityComponent.generated.h"


class ARPGTargetingPreviewActor;
class URPGAbilitySummon;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityUsed, URPGAbilityBase*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityChannelStarted, URPGAbilityBase*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityChannelStopped, URPGAbilityBase*, Ability, EAbilityInterruptReason, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityToggled, URPGAbilityBase*, Ability, bool, bToggled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTargetingPreviewStarted, URPGAbilityBase*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTargetingPreviewConfirmed, URPGAbilityBase*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTargetingPreviewCanceled, URPGAbilityBase*, Ability);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URPGAbilityComponent();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Trace")
	void TraceForTargetData(const float InTraceLength, URPGAbilityBase* Ability, FRPGTargetData& OutTargetData);
	FRPGTargetData TraceForTargetData(const float InTraceLength, URPGAbilityBase* Ability);
	
	UFUNCTION(BlueprintCallable)
	void SetTimerForAbilityCooldownExpiration(URPGAbilityBase* Ability);	
	
	UFUNCTION()
	void OnCooldownTimerExpired(URPGAbilityBase* Ability);
	
	UFUNCTION(BlueprintCallable)
	FTimerHandle SetTimerForCastAbility(URPGAbilityBase* Ability, FRPGTargetData TargetData);
	
	// TODO: Might need to remove this function since it was created for bypassing blueprints limitation
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
	TArray<FHitResult> QuerySphereTargets(FVector SweepStart, FVector SweepEnd, float SweepRadius);
	
	UFUNCTION(BlueprintCallable)
	FHitResult QueryLinetrace(FVector Start, FVector End);
	
	UFUNCTION(BlueprintCallable)
	void StartChannel(URPGAbilityBase* Ability, FRPGTargetData TargetData);
	
	UFUNCTION(BlueprintCallable)
	void UpdateChannels();
	
	UFUNCTION(BlueprintCallable)
	void StopChannel(URPGAbilityBase* Ability, EAbilityInterruptReason Reason);
	
	UFUNCTION(BlueprintCallable)
	bool HasActiveAbilities() const;
	
	UFUNCTION(BlueprintCallable)
	float GetChannelDurationPercentForAbility(URPGAbilityBase* Ability) const;
	
	UFUNCTION(BlueprintCallable)
	float GetChannelRemainingTimeForAbility(URPGAbilityBase* Ability) const;
	
	UFUNCTION(BlueprintCallable)
	void StartCast(URPGAbilityBase* Ability, FRPGTargetData TargetData);
	
	UFUNCTION(BlueprintCallable)
	void FinishCast(URPGAbilityBase* Ability, FRPGTargetData TargetData);
	
	UFUNCTION(BlueprintCallable)
	void InterruptCast(URPGAbilityBase* Ability, EAbilityInterruptReason Reason, FRPGTargetData TargetData);
	
	UFUNCTION(BlueprintCallable)
	float GetCastDurationPercentForAbility(URPGAbilityBase* Ability) const;
	
	UFUNCTION(BlueprintCallable)
	float GetCastRemainingTimeForAbility(URPGAbilityBase* Ability) const;
	
	UFUNCTION(BlueprintCallable)
	void StartToggle(URPGAbilityBase* Ability, FRPGTargetData TargetData);
	
	UFUNCTION(BlueprintCallable)
	void StopToggle(URPGAbilityBase* Ability, FRPGTargetData TargetData);

	UFUNCTION(BlueprintCallable)
	void BeginTargetingPreview(URPGAbilityBase* Ability);
	
	UFUNCTION(BlueprintCallable)
	void UpdateTargetingPreview();
	
	UFUNCTION(BlueprintCallable)
	void CancelTargetingPreview();
	
	UFUNCTION(BlueprintCallable)
	void ConfirmTargetingPreview();
	
	// TODO: Review params
	UFUNCTION(BlueprintCallable)
	void SpawnPreviewActor(URPGAbilitySummon* SummonAbility);
	
	UFUNCTION(BlueprintCallable)
	AActor* SpawnSummonActor(TSubclassOf<AActor> ClassToSpawn, FTransform SpawnTransform);
	
	UFUNCTION(BlueprintCallable)
	void TryUsingAbility(const int32 AbilityArrayIndex);
	
	UFUNCTION(BlueprintCallable)
	void TryInterruptingActiveAbilities();
	
	UFUNCTION(BlueprintCallable)
	void StartCooldown(URPGAbilityBase* Ability);
	
	UFUNCTION(BlueprintCallable)
	bool IsAbilityOnCooldown(URPGAbilityBase* Ability) const;
	
	UFUNCTION(BlueprintCallable)
	void AddAbility(URPGAbilityDefinitionData* NewAbilityDefinition);
	
	UFUNCTION(BlueprintCallable)
	void InitAbilities();
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityCooldownStarted OnAbilityCooldownStarted;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityCooldownEnded OnAbilityCooldownEnded;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityAdded OnAbilityAdded;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityUsed OnAbilityUsed;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityChannelStarted OnAbilityChannelStarted;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityChannelStopped OnAbilityChannelStopped;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityToggled OnAbilityToggled;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityTargetingPreviewStarted OnAbilityTargetingPreviewStarted;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityTargetingPreviewConfirmed OnAbilityTargetingPreviewConfirmed;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAbilityTargetingPreviewCanceled OnAbilityTargetingPreviewCanceled;
	
	// TODO: Change property params later on to be more read only
	/* Abilities to spawn from. Literally a set of abilities that will be spawned upon starting a game for specific component*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<URPGAbilityDefinitionData*> TemplateAbilityDefinitions;
	
	/* Spawned abilities for this component, mapped to a spawned object */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TMap<URPGAbilityDefinitionData*, URPGAbilityBase*> SpawnedAbilityDefinitions;
	
	// TODO: Not sure that we need to have multiple channels, since a cast can only be one, why several channels then?
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<URPGAbilityBase*> ActiveChannels;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	URPGAbilityBase* ActiveCast;
	
	UPROPERTY()
	FTimerHandle ActiveAbilityUpdateTimer;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bIsInTargetingPreview;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	URPGAbilityBase* PendingAbility;
	
	UPROPERTY()
	FTimerHandle TargetingPreviewTimer;
	
	UPROPERTY(EditDefaultsOnly, Category = "Targeting Preview")
	float TargetingPreviewUpdateRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Targeting Preview")
	TSubclassOf<ARPGTargetingPreviewActor> TargetingPreviewActorClass;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FRPGTargetData TargetingPreviewTargetData;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	ARPGTargetingPreviewActor* TargetingPreviewActor;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TArray<UMaterialInstanceDynamic*> TargetingPreviewActorMIDs; 
	
private:
	
	URPGAbilityBase* SpawnAbilityObject(URPGAbilityDefinitionData* AbilityDefinition);
	
	UFUNCTION()
	void OnOwnerCharacterMoved();
	
};
