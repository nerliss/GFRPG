// Oleksandr Tkachov 2022-2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGAbilityDefinitionData.generated.h"

class ARPGProjectileBase;
class URPGAbilityBase;
class UParticleSystem;
class UTexture2D;

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
	float CastTime;
	
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

