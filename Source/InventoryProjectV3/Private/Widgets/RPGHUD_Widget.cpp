// Oleksandr Tkachov 2021-2026


#include "Widgets/RPGHUD_Widget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/RPGInteractionPrompt_Widget.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

void URPGHUD_Widget::DisplayInteractionMessage(bool bShowMessage, FText TargetName)
{
	if (!InteractionPrompt_WidgetClass)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGUIHUD, Error, TEXT("InteractionPrompt_WidgetClass is nullptr!"));
		return;
	}

	if (!HUDCanvas)
	{
		LOG_WITH_FUNCTION_NAME(LogRPGUIHUD, Error, TEXT("HUDCanvas doesn't exist!"));
		return;
	}

	if (!bShowMessage)
	{
		if (InteractionPrompt_Widget)
		{
			InteractionPrompt_Widget->RemoveFromParent();
			InteractionPrompt_Widget = nullptr;
		}

		return;
	}

	if (!InteractionPrompt_Widget)
	{
		InteractionPrompt_Widget = Cast<URPGInteractionPrompt_Widget>(CreateWidget(GetWorld(), InteractionPrompt_WidgetClass));
		InteractionPrompt_Widget->Text_Target_Name = TargetName;

		HUDCanvas->AddChildToCanvas(InteractionPrompt_Widget);

		// This will probably get us the most recently-created slot (last slot)
		const int32 LastSlot = HUDCanvas->GetChildrenCount() - 1;
		UCanvasPanelSlot* InteractionPromptSlot = CastChecked<UCanvasPanelSlot>(HUDCanvas->GetSlots()[LastSlot]);

		// TODO: Should probably move this to a config or something
		// Approximately lower center of the screen
		InteractionPromptSlot->SetAutoSize(true);
		InteractionPromptSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		InteractionPromptSlot->SetAlignment(FVector2D(0.5f, -1.f));
		InteractionPromptSlot->SetPosition(FVector2D(0.f, 0.f));
	}
}
