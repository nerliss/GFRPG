// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quests/RPGQuest.h"
#include "RPGQuestHUDObjectiveEntryWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGQuestHUDObjectiveEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void UpdateTrackerData();

	FText GetTrackerData() const; 

	FText GetQuestStatusText() const;
	
	FObjectiveData Objective;

	bool bQuestComplete;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FText CompletionText;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FSlateColor IncompleteColor;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FText StatusText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UTextBlock* ObjectiveTrackerTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UTextBlock* ObjectiveStatusTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UTextBlock* ObjectiveDescriptionTextBlock;
	
};
