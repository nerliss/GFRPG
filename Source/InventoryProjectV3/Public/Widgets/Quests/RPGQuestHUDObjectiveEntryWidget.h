// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quests/RPGQuest.h"
#include "RPGQuestHUDObjectiveEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGQuestHUDObjectiveEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void UpdateTrackerData();
	
	FObjectiveData Objective;

	bool bQuestComplete;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FText CompletionText;
		
};
