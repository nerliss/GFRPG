// Oleksandr Tkachov 2022-2023


#include "Widgets/RPGHUD_Widget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/RPGInteractionPrompt_Widget.h"
#include "Utility/Utility.h"
#include "Utility/LogDefinitions.h"

bool URPGHUD_Widget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return false;
	}

	// MyTODO: Create references to multiple user-created widgets that still need to be implemented via C++

	return true;
}

void URPGHUD_Widget::DisplayInteractionMessage(bool bShowMessage, FText TargetName)
{
	if (!InteractionPrompt_WidgetClass)
	{
		UE_LOG(LogRPGUIHUD, Error, TEXT("[URPGHUD_Widget::DisplayInteractionMessage] InteractionPrompt_WidgetClass is nullptr!"));
		return;
	}

	if (!HUDCanvas)
	{
		UE_LOG(LogRPGUIHUD, Error, TEXT("[URPGHUD_Widget::DisplayInteractionMessage] HUDCanvas doesn't exist!"));
		return;
	}

	// Destroy message if it exists
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

		// MyTODO: Should probably move this to a config or something
		// Approximately lower center of the screen
		InteractionPromptSlot->SetAutoSize(true);
		InteractionPromptSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		InteractionPromptSlot->SetAlignment(FVector2D(0.5f, -1.f));
		InteractionPromptSlot->SetPosition(FVector2D(0.f, 0.f));
	}
}
