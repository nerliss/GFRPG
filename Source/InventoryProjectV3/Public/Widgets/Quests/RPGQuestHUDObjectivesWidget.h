// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestHUDObjectivesWidget.generated.h"

class URPGQuestHUDObjectiveEntryWidget;
struct FObjectiveData;
class UTextBlock;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGQuestHUDObjectivesWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void UpdateObjectiveList();
	
protected:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UVerticalBox* ObjectivesListBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UTextBlock* QuestName;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TSubclassOf<URPGQuestHUDObjectiveEntryWidget> ObjectiveHUDEntryWidgetClass;

private:
	
	TArray<FObjectiveData> Objectives;
	
};
