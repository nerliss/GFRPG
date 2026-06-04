// Oleksandr Tkachov 2021-2026

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
	Instant			UMETA(DisplayName = "Instant"),
	Toggle			UMETA(DisplayName = "Toggle"),
	Channel			UMETA(DisplayName = "Channel"),
	Cast			UMETA(DisplayName = "Cast"),
	Max				UMETA(Hidden)
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

UENUM(BlueprintType)
enum class EAbilityStatToModifyType : uint8
{
	// TODO: Reuse from Stats system once that is implemented
	Health,
	Resource,
	Strength,
	Agility,
	Intellect,
	Damage,
	XP,
	Reputation,
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EStatModificationType : uint8
{
	Flat,			// A flat number to modify a stat by (+50, -33 etc.)
	Percentage,		// A percentage of that stat to modify it by (modify Health by 20% of that Max Health amount) 
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EStatModificationOperation : uint8
{
	Add,			
	Subtract,		
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cast", meta=(ClampMin="0.0"))
	float CastTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cast")
	bool bLockTargetAtStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cast")
	bool bInterruptOnMove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cast")
	bool bPayCostOnStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cast")
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode", meta = (EditCondition = "ActivationMode == EAbilityActivationMode::Channel"))
	FChannelParams ChannelParams;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode", meta = (EditCondition = "ActivationMode == EAbilityActivationMode::Cast"))
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

UCLASS()
class INVENTORYPROJECTV3_API URPGModifyStatAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	URPGModifyStatAbilityDefinitionData();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modify Stat Data")
	EAbilityStatToModifyType StatToModify;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modify Stat Data")
	EStatModificationType StatModificationType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modify Stat Data")
	EStatModificationOperation StatModificationOperation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modify Stat Data", meta = (EditCondition = "StatModificationType == EStatModificationType::Flat"))
	float AmountToModify;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modify Stat Data", meta = (EditCondition = "StatModificationType == EStatModificationType::Percentage"))
	float PercentageToModify;
};

UCLASS()
class INVENTORYPROJECTV3_API URPGDisguiseAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	URPGDisguiseAbilityDefinitionData();
	
	
	
};

UCLASS()
class INVENTORYPROJECTV3_API URPGDisplaceAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	URPGDisplaceAbilityDefinitionData();
	
	
	
};

UCLASS()
class INVENTORYPROJECTV3_API URPGGiveItemAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	URPGGiveItemAbilityDefinitionData();
	
	
	
};

