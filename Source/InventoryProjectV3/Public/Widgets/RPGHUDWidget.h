// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGHUDWidget.generated.h"

class URPGHPBarWidget;
class URPGQuestHUDObjectivesWidget;
class UCanvasPanel;
class URPGInteractionPromptWidget;
class URPGXPBarWidget;
class URPGMiniMapWidget;

UCLASS()
class INVENTORYPROJECTV3_API URPGHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/** 
	* bShowMessage = true creates an InteractionPrompt_Widget,
	* bShowMessage = false destroys the widget and nullifies its reference
	* Use bShowMessage = false if you want to destroy already existing message
	*/
	// TODO: Not sure this is supposed to be a public exposed to blueprints
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void DisplayInteractionMessage(bool bShowMessage, FText TargetName);

	/* Interaction Prompt Widget Class to spawn from */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClass")
	TSubclassOf<URPGInteractionPromptWidget> InteractionPrompt_WidgetClass;
	
	UPROPERTY()
	URPGInteractionPromptWidget* InteractionPrompt_Widget;

	/* Quest HUD Objectives Class to spawn from */
	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<URPGQuestHUDObjectivesWidget> QuestHUDObjectivesWidgetClass;

	/* Main canvas */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UCanvasPanel* HUDCanvas;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGQuestHUDObjectivesWidget* QuestHUDObjectivesWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGXPBarWidget* XPBarWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGHPBarWidget* HPBarWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGMiniMapWidget* MiniMapWidget;
	
};
