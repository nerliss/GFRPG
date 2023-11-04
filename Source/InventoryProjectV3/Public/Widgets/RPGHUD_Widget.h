// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGHUD_Widget.generated.h"

class UCanvasPanel;
class URPGInteractionPrompt_Widget;
class URPGXPBar_Widget;
class URPGMiniMapWidget;

UCLASS()
class INVENTORYPROJECTV3_API URPGHUD_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	bool Initialize() override;

public:

	// Canvas is used to store different user-created widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* HUDCanvas;

	// In BP set to BP child of RPGInteractionPrompt_Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClassTypes")
	TSubclassOf<UUserWidget> InteractionPrompt_WidgetClass;

 	// Widget reference
	UPROPERTY()
	URPGInteractionPrompt_Widget* InteractionPrompt_Widget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	URPGMiniMapWidget* MiniMapWidget;

	/** 
	* bShowMessage = true creates an InteractionPrompt_Widget,
	* bShowMessage = false destroys the widget and nullifies its reference
	* Use bShowMessage = false if you want to destroy already existing message
	*/
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void DisplayInteractionMessage(bool bShowMessage, FText TargetName);
	
};
