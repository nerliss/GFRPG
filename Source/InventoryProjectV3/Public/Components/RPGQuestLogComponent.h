// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGQuestLogComponent.generated.h"


class ARPGCharacter;
class ARPGQuest;
class URPGQuestLogWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGQuestLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	URPGQuestLogComponent();
	
	virtual void BeginPlay() override;

	TArray<ARPGQuest*> GetActiveQuests() const { return ActiveQuests; }
	
	TArray<ARPGQuest*> GetCompleteQuests() const { return CompleteQuests; }

	ARPGQuest* GetCurrentActiveQuest() const { return CurrentActiveQuest; }

	void SetActiveQuest(ARPGQuest* Quest, bool bPlaySound = false);

	void AddQuest(ARPGQuest* Quest);

	bool IsQuestAccepted(TSubclassOf<ARPGQuest> QuestClass) const;

	bool IsQuestComplete(TSubclassOf<ARPGQuest> QuestClass) const;
	
	void CheckPlayerInventory(ARPGQuest* Quest);

	bool MarkQuestComplete(TSubclassOf<ARPGQuest> QuestClass) const;

	void DeleteQuest (ARPGQuest* Quest, bool bAbandonQuest = false);

	void ToggleQuestLog();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TArray<ARPGQuest*> ActiveQuests;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TArray<ARPGQuest*> CompleteQuests;

	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	ARPGQuest* CurrentActiveQuest;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TSubclassOf<URPGQuestLogWidget> QuestLogWidgetClass;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	URPGQuestLogWidget* QuestLogWidget;

private:

	/* Returns first quest actor in world
	 * TODO: Temporary function that should be replaced when there is a better way to access spawned quest  
	 */
	ARPGQuest* GetSpawnedQuest(TSubclassOf<ARPGQuest> QuestClass) const;
	
};
