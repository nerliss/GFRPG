// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quests/RPGQuest.h"
#include "RPGQuestObjectiveItemWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGQuestObjectiveItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FObjectiveData Objective;
	
	// TODO: Remove this since we can directly access the text from Objective above
	// UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	// bool bCompleted;
	// UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	// FText ObjectiveText;
	
protected:

	virtual void NativeConstruct() override;
	
	void UpdateTrackerData();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	UTextBlock* ObjectiveDescriptionText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	UTextBlock* ObjectiveStatusText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	UTextBlock* ObjectiveTrackerText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FSlateColor CompleteColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FSlateColor ActiveColor;
	
};
