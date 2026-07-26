// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGXPComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelGained);

class USoundBase;
class UParticleSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGXPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URPGXPComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Used for XP Progress Bar in UI because PBs take values only between 0 and 1 */
	void CalculateXPPercentage();

	/** Used for Buffer Progress Bar in UI because PBs take values only between 0 and 1 */
	void CalculateXPBufferPercentage();

	void CalculateMaxXP();

	UFUNCTION(BlueprintCallable, Category = "XP|Functions")
	void AddXP(float AddedXP);

	void LevelUp();

	/** Allows to calculate rewards for killing mobs and completing quests */
	UFUNCTION(BlueprintCallable, Category = "XP|Functions")
	float CalculateXPReward(bool bQuestReward, float Multiplier = 1.f);

	float GetCurrentXP() const { return CurrentXP; }
	float GetCurrentMaxXP() const { return CurrentMaxXP; } 
	float GetCurrentXPPercentage() const { return XPCurrentPercentage; }
	float GetCurrentPercentageBuffer() const { return BufferXPCurrentPercentage; }
	
	int32 GetCurrentLevel() const { return CurrentLevel; }
	int32 GetSkillPoints() const { return SkillPoints; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelGained OnLevelGained;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XP|Sounds")
	USoundBase* LevelUpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XP|Particles")
	UParticleSystem* LevelUpParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|Level")
	int32 CurrentLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XP|Level")
	int32 CapLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|SP")
	int32 SkillPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|Experience")
	float CurrentXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|Experience")
	float CurrentMaxXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|Experience")
	float BufferXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XP|Experience")
	float XPCurrentPercentage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XP|Experience")
	float BufferXPCurrentPercentage;
	
};
