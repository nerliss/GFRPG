// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGQuest.generated.h"

class URPGQuestLogEntryWidget;
class ARPGPlayerController;
class ARPGQuestMarkerLocation;
class ARPGItemBase;
class ARPGCharacter;
class UStaticMeshComponent;

UENUM(Blueprintable, BlueprintType)
enum EObjectiveType
{
	OT_Location UMETA(DisplayName = "Location"),
	OT_Interact UMETA(DisplayName = "Interact"),
	OT_Collect  UMETA(DisplayName = "Collect"),
	OT_Kill		UMETA(DisplayName = "Kill"),
	OT_Max		UMETA(Hidden)
};

USTRUCT(Blueprintable, BlueprintType)
struct FObjectiveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EObjectiveType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<AActor> Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;

	FObjectiveData()
	{
		Description = FText::GetEmpty();
		Type = OT_Max;
		bCompleted = false;
		bCanBeCompleted = false;
		Target = nullptr;
		Amount = 0;
	};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetInteracted, AActor*, InteractionTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLocationReached, ARPGQuestMarkerLocation*, LocationTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemCollected, ARPGItemBase*, ItemTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, ARPGCharacter*, KillTarget);

UCLASS()
class INVENTORYPROJECTV3_API ARPGQuest : public AActor
{
	GENERATED_BODY()

public:

	ARPGQuest();
	
	TArray <FObjectiveData> GetObjectives() const { return Objectives; }

	int32 GetKillCountCurrent() const { return KillCountCurrent; }
	
	bool GetActiveObjective(int32& ObjectiveIndex, FObjectiveData& Objective) const;

	bool GetNextObjective(int32& ObjectiveIndex, FObjectiveData& Objective) const;

	FText GetQuestName() const { return Name; }

	float GetXPReward() const { return XPReward; }
	
	/* In BP version this was called when player gained new level to adjust XP reward for the new level thus making quests always relevant
	 * TODO: Review this logic and adjust - change it to delegates or something (because this is called in Player Character class when player levels up)
	 */
	void CalculateXP();

	void OnQuestCompleted();
	
	// TODO: Check blueprint access specifiers - may be we don't need any
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable, Category = "Quest")
	FOnTargetInteracted OnTargetInteracted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FOnLocationReached OnLocationReached;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FOnItemCollected OnItemCollected;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FOnEnemyKilled OnEnemyKilled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	bool bStoryQuest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	bool bCanBeTurnedInToSomeoneElse;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	bool bCompleted;
	
	// TODO: Probably should only be available if bCanBeTurnedInToSomeoneElse is true. Also set this to autofill with QuestGiver if set to false
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<AActor> QuestTurnInTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<AActor> QuestGiver;
	
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	URPGQuestLogEntryWidget* QuestLogEntryRef;
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckInteractionObjective(AActor* InteractionTarget);

	UFUNCTION()
	void CheckLocationObjective(ARPGQuestMarkerLocation* LocationTarget);

	UFUNCTION()
	void CheckItemObjective(ARPGItemBase* ItemTarget);

	UFUNCTION()
	void CheckKillObjective(ARPGCharacter* KillTarget);

	void MakeNearestObjectiveAvailable(int32 ObjectiveIndex);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	UStaticMeshComponent* QuestMarkComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray <FObjectiveData> Objectives;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	int32 KillCountCurrent;

	// TODO: Next bottom three should be a struct called QuestRewards or something. Could also include some Item rewards or any other custom reward
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	float XPReward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	float XPRewardMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	float MoneyReward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FText TurnInText;

	// TODO: Maybe use TSubclassOf<ARPGQuest> or TSoftObjectPtr/TSoftClassPtr?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	ARPGQuest* Prerequisite;

private:

	void UpdateUI(bool bNeedsUIUpdate);
	
};
