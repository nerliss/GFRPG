// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestLogEntryWidget.generated.h"

class ARPGQuest;

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGQuestLogEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	ARPGQuest* Quest;
	
};
