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
	AAM_Instant,
	AAM_Toggle,
	AAM_Channel,
	AAM_Max UMETA(Hidden)
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
	
	FChannelParams()
	{
		ChannelDuration = 3.0f;
		TickPeriod = 0.25f;
		bTickOnStart = true;
		bRequiresButtonHold = false;
		bUpdateTargetEachTick = false;
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode")
	EAbilityActivationMode ActivationMode;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation Mode", meta = (EditCondition = "ActivationMode == EAbilityActivationMode::AAM_Channel"))
	FChannelParams ChannelParams;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	UParticleSystem* ParticleSystem;
	
	
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

