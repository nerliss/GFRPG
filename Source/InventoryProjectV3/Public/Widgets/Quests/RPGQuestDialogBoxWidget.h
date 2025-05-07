// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestDialogBoxWidget.generated.h"

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
	
	void OnTurnedInClicked();
	void OnAcceptClicked();

	void RemoveWidget();
	
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	ARPGQuest* Quest;
	
};
