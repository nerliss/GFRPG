// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestDialogBoxWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGQuestDialogBoxWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
};
