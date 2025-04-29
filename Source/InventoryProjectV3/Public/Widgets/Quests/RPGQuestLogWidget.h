// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestLogWidget.generated.h"

class URPGQuestLogEntryWidget;
class UVerticalBox;
class URPGQuestObjectiveItemWidget;
class UScrollBox;
class URPGQuestLogComponent;
class ARPGQuest;

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGQuestLogWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void OnActiveQuestChanged();
	
protected:

	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UScrollBox* ObjectiveList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UScrollBox* StoryQuestList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UScrollBox* SideQuestList;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UVerticalBox* RewardBox; 
	
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	URPGQuestLogComponent* PlayerQuestLog;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	ARPGQuest* ActiveQuest;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TSubclassOf<URPGQuestObjectiveItemWidget> ObjectiveItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TSubclassOf<URPGQuestLogEntryWidget> QuestLogEntryWidgetClass;

private:

	void RefreshQuestLogUI();
	void PlayConstructionFX();
};
