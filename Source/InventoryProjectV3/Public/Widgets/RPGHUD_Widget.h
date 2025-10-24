// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGHUD_Widget.generated.h"

class URPGHPBar_Widget;
class URPGQuestHUDObjectivesWidget;
class UCanvasPanel;
class URPGInteractionPrompt_Widget;
class URPGXPBar_Widget;
class URPGMiniMapWidget;

UCLASS()
class INVENTORYPROJECTV3_API URPGHUD_Widget : public UUserWidget
{
	GENERATED_BODY()

	// TODO: Not sure we need this
	virtual bool Initialize() override;

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
	TSubclassOf<URPGInteractionPrompt_Widget> InteractionPrompt_WidgetClass;
	
	UPROPERTY()
	URPGInteractionPrompt_Widget* InteractionPrompt_Widget;

	/* Quest HUD Objectives Class to spawn from */
	UPROPERTY(EditDefaultsOnly, Category = "WidgetClass")
	TSubclassOf<URPGQuestHUDObjectivesWidget> QuestHUDObjectivesWidgetClass;

	/* Main canvas */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UCanvasPanel* HUDCanvas;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGQuestHUDObjectivesWidget* QuestHUDObjectivesWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGXPBar_Widget* XPBarWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGHPBar_Widget* HPBarWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URPGMiniMapWidget* MiniMapWidget;
	
};
