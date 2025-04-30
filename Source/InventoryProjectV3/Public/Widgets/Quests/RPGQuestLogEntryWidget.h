// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGQuestLogEntryWidget.generated.h"

class ARPGQuest;
class USlateBrushAsset;
class UButton;
class UBorder;

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

protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnEntryButtonClicked();
	
	FSlateBrush GetBorderStatus() const;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UBorder* EntryBorder;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Quest")
	UButton* EntryButton;
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	USlateBrushAsset* ActiveSlateBrush;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	USlateBrushAsset* InactiveSlateBrush;
	
};
