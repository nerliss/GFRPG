// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGXP_Component.generated.h"

// Custom event for Level up
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelGained);

class USoundBase;
class UParticleSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGXP_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGXP_Component();

	/** Used for XP Progress Bar in UI because PBs take values only between 0 and 1 */
	void Calculate_Percentage_XP();

	/** Used for Buffer Progress Bar in UI because PBs take values only between 0 and 1 */
	void Calculate_Percentage_Buffer();

	void Calculate_MaxXP();

	UFUNCTION(BlueprintCallable, Category = "XP|Functions")
	void AddXP(float AddedXP);

	void LevelUp();

	/** Allows to calculate rewards for killing mobs and completing quests */
	UFUNCTION(BlueprintCallable, Category = "XP|Functions")
	float CalculateXPReward(bool bQuestReward, float Multiplier = 1.f);

	float GetXP_Current();

	float GetXP_Current_Max();

	float GetCurrentPercentage_XP();

	float GetCurrentPercentage_Buffer();

	int32 GetSkillPoints();

	int32 GetLevelCurrent();

	/** Create an instance of OnLevelGained event */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelGained OnLevelGained;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XP|Sounds")
	USoundBase* LevelUpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "XP|Particles")
	UParticleSystem* LevelUpParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|Level")
	int32 Level_Current;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XP|Level")
	int32 Level_Cap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|SP")
	int32 SkillPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|Experience")
	float XP_Current;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|Experience")
	float XP_Current_Max;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "XP|Experience")
	float XP_Buffer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XP|Experience")
	float CurrentPercentage_XP;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "XP|Experience")
	float CurrentPercentage_Buffer;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
