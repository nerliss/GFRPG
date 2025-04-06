// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGQuest.generated.h"

class ARPGPlayer_Controller;
class ARPGQuestMarkerLocation;
class ARPGItem_Base;
class ARPGCharacter;

UENUM(Blueprintable, BlueprintType)
enum EObjectiveType
{
	OT_Location,
	OT_Interact,
	OT_Collect,
	OT_Kill,
	OT_Max
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

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetInteracted, AActor*, InteractionTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLocationReached, ARPGQuestMarkerLocation*, LocationTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemCollected, ARPGItem_Base*, ItemTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, ARPGCharacter*, KillTarget);

UCLASS()
class INVENTORYPROJECTV3_API ARPGQuest : public AActor
{
	GENERATED_BODY()

public:

	ARPGQuest();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TArray <FObjectiveData> Objectives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	bool bStoryQuest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	bool bCompleted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
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

	// QuestLogEntryRef - Requires Widget C++ Class

	// TODO: Maybe use TSubclassOf<ARPGQuest> or TSoftObjectPtr/TSoftClassPtr?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	ARPGQuest* Prerequisite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<AActor> QuestGiver;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	bool bCanBeTurnedInToSomeoneElse;

	// TODO: Probably should only be available if bCanBeTurnedInToSomeoneElse is true. Also set this to autofill with QuestGiver if set to false
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<AActor> QuestTurnInTarget;

	// TODO: Not sure if this is supposed to be a soft obj since player is always loaded
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<ACharacter> PlayerRef;

	// TODO: Not sure about soft obj
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<ARPGPlayer_Controller> PCRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FOnTargetInteracted OnTargetInteracted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FOnEnemyKilled OnEnemyKilled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FOnItemCollected OnItemCollected;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FOnLocationReached OnLocationReached;

};
