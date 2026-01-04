// Oleksandr Tkachov 2022-2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGAbilityDefinitionData.generated.h"

class URPGEffectDefinitionData;
class ARPGProjectileBase;
class URPGAbilityBase;
class UParticleSystem;
class UTexture2D;

UENUM(BlueprintType)
enum class EAbilityActivationMode : uint8
{
	AAM_Instant			UMETA(DisplayName = "Instant"),
	AAM_Toggle			UMETA(DisplayName = "Toggle"),
	AAM_Channel			UMETA(DisplayName = "Channel"),
	AAM_Cast			UMETA(DisplayName = "Cast"),
	AAM_Max				UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAbilityTargetingFlow : uint8
{
	Instant,
	PreviewConfirm,
	HoldRelease,
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAbilityInterruptReason : uint8
{
	DurationEnd,
	Interrupt,
	InputRelease,
	Moved,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FChannelParams
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Channel")
	float ChannelDuration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Channel")
	float TickPeriod;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Channel")
	bool bTickOnStart;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Channel")
	bool bRequiresButtonHold;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Channel")
	bool bUpdateTargetEachTick;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Channel")
	bool bInterruptOnMove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bStartCooldownOnStart;
	
	FChannelParams()
	{
		ChannelDuration = 3.0f;
		TickPeriod = 0.25f;
		bTickOnStart = true;
		bRequiresButtonHold = false;
		bUpdateTargetEachTick = false;
		bInterruptOnMove = true;
		bStartCooldownOnStart = false;
	}
};

USTRUCT(BlueprintType)
struct FCastParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin="0.0"))
	float CastTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bLockTargetAtStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bInterruptOnMove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPayCostOnStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bStartCooldownOnStart;
	
	FCastParams()
	{
		CastTime = 3.f;
		bLockTargetAtStart = true;
		bInterruptOnMove = true;
		bPayCostOnStart = false;
		bStartCooldownOnStart = false;
	}
};

/**
 * Ability Definition Data base class. Allows the Ability System to be Data-driven. Creating a derived class basically means to create a new ability
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityDefinitionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	URPGAbilityDefinitionData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	TSubclassOf<URPGAbilityBase> AbilityClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	UTexture2D* Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FText Description;
	
	/* How long will ability be unusable after successful cast? 0 means no cooldown. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	float Cooldown;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	float Cost;
	
	/* How far TargetData for this ability can be gathered from? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	float CastRange;
	
	/* Perform a ground trace from original target trace's end location to try getting Target Data even if the cast thrown in air and didn't hit anything */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	bool bDoGroundTrace;
	
	/* Distance to trace below from original target trace's end */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System", meta = (EditCondition = "bDoGroundTrace"))
	float GroundTraceDistance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	UParticleSystem* ParticleSystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting Flow")
	EAbilityTargetingFlow TargetingFlow;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode")
	EAbilityActivationMode ActivationMode;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode", meta = (EditCondition = "ActivationMode == EAbilityActivationMode::AAM_Channel"))
	FChannelParams ChannelParams;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode", meta = (EditCondition = "ActivationMode == EAbilityActivationMode::AAM_Cast"))
	FCastParams CastParams;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TArray<URPGEffectDefinitionData*> EffectsToApplyOnTarget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TArray<URPGEffectDefinitionData*> EffectsToApplyOnSelf;
	
};

UCLASS()
class INVENTORYPROJECTV3_API URPGSummonAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	URPGSummonAbilityDefinitionData();
	
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summon Data")
	TSubclassOf<AActor> ActorToSpawn;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Summon Data")
	USkeletalMesh* PreviewMesh;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Summon Data")
	TSubclassOf<UAnimInstance> PreviewAnimClass;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Summon Data")
	FVector SpawnScale;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summon Data")
	UMaterial* PreviewMaterial;
};

UCLASS()
class INVENTORYPROJECTV3_API URPGAOEAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	URPGAOEAbilityDefinitionData();
	
	
	
};

UCLASS()
class INVENTORYPROJECTV3_API URPGProjectileAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	URPGProjectileAbilityDefinitionData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Data")
	TSubclassOf<ARPGProjectileBase> ProjectileToSpawn;
	
};

