// Destruction Games. 2022


#include "Widgets/RPGHUD_Widget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/RPGInteractionPrompt_Widget.h"
#include "Utility/Utility.h"

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

void URPGHUD_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void URPGHUD_Widget::DisplayInteractionMessage(bool bShowMessage, FText TargetName)
{
	// If class is set in BP
	if (InteractionPrompt_WidgetClass)
	{
		// Show message
		if (bShowMessage)
		{
			if (!InteractionPrompt_Widget)
			{
				InteractionPrompt_Widget = Cast<URPGInteractionPrompt_Widget>(CreateWidget(GetWorld(), InteractionPrompt_WidgetClass));

				InteractionPrompt_Widget->Text_Target_Name = TargetName;

				// Configure the canvas panel slot for interaction message
				if (HUDCanvas)
				{
					HUDCanvas->AddChildToCanvas(InteractionPrompt_Widget);

					// This will probably get us the most recently-created slot (last slot)
					int32 LastSlot = HUDCanvas->GetChildrenCount() - 1;

					// Should dynamically find the last slot
					UCanvasPanelSlot* InteractionPromptSlot = CastChecked<UCanvasPanelSlot>(HUDCanvas->GetSlots()[LastSlot]);

					// Approximately lower center of the screen
					InteractionPromptSlot->SetAutoSize(true);
					InteractionPromptSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
					InteractionPromptSlot->SetAlignment(FVector2D(0.5f, -1.f));
					InteractionPromptSlot->SetPosition(FVector2D(0.f, 0.f));
				}
			}
		}
		// Delete message
		else
		{
			if (InteractionPrompt_Widget)
			{
				InteractionPrompt_Widget->RemoveFromParent();
				InteractionPrompt_Widget = nullptr;
			}
		}
	}
}
