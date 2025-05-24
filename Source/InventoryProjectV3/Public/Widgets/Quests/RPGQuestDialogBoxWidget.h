// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestDialogBoxWidget.generated.h"

class UScrollBox;
class URPGQuestObjectiveItemWidget;
class ARPGQuest;

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGQuestDialogBoxWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	bool CheckPlayerInventory() const;

	void RemoveQuestItemsFromInventory();

	UFUNCTION(BlueprintCallable)
	void OnTurnInClicked();

	UFUNCTION(BlueprintCallable)
	void OnAcceptClicked();

	UFUNCTION(BlueprintCallable)
	void RemoveWidget();

	void PlayConstructFX();
	void PlayDestructFX();
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Quest")
	UScrollBox* ObjectiveListBox;
	
	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	ARPGQuest* Quest;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TSubclassOf<URPGQuestObjectiveItemWidget> QuestObjectiveItemWidgetClass;
	
};
