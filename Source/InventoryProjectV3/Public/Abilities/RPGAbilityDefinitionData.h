// Oleksandr Tkachov 2022-2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGAbilityDefinitionData.generated.h"

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
	
	FChannelParams()
	{
		ChannelDuration = 3.0f;
		TickPeriod = 0.25f;
		bTickOnStart = true;
		bRequiresButtonHold = false;
		bUpdateTargetEachTick = false;
		bInterruptOnMove = true;
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
		bStartCooldownOnStart = true;
	}
};

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityDefinitionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	TSubclassOf<URPGAbilityBase> AbilityClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	UTexture2D* Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	float Cooldown;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	float Cost;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	float CastRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	UParticleSystem* ParticleSystem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode")
	EAbilityActivationMode ActivationMode;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode", meta = (EditCondition = "ActivationMode == EAbilityActivationMode::AAM_Channel"))
	FChannelParams ChannelParams;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode", meta = (EditCondition = "ActivationMode == EAbilityActivationMode::AAM_Cast"))
	FCastParams CastParams;
};

UCLASS()
class INVENTORYPROJECTV3_API URPGSummonAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	TSubclassOf<AActor> ActorToSpawn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FTransform SpawnTransform;
	
};

UCLASS()
class INVENTORYPROJECTV3_API URPGAOEAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	
};

UCLASS()
class INVENTORYPROJECTV3_API URPGProjectileAbilityDefinitionData : public URPGAbilityDefinitionData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	TSubclassOf<ARPGProjectileBase> ProjectileToSpawn;
	
};

