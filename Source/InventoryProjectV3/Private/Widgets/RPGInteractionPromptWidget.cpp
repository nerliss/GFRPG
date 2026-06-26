// Oleksandr Tkachov 2021-2026

#include "Widgets/RPGInteractionPromptWidget.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

bool URPGInteractionPromptWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	Text_Message = FText::FromString(TEXT("Press E to interact"));
	Text_Target_Name = FText::FromString(TEXT("Target name"));

	if (MainSizeBox)
	{
		MainSizeBox->SetMinDesiredHeight(100.f);
		MainSizeBox->SetMinDesiredWidth(300.f);
	}

	if (UpperBorder)
	{
		UVerticalBoxSlot* UpperBorderBoxSlot = CastChecked<UVerticalBoxSlot>(MainVerticalBox->GetSlots()[0]);
		
		// Set size to fill, 1.f
		UpperBorderBoxSlot->SetSize(FSlateChildSize());
		
		UpperBorderBoxSlot->SetHorizontalAlignment(HAlign_Fill);
		UpperBorderBoxSlot->SetVerticalAlignment(VAlign_Fill);

		UpperBorder->SetHorizontalAlignment(HAlign_Center);
		UpperBorder->SetVerticalAlignment(VAlign_Center);

		UpperBorder->SetBrushColor(FLinearColor(1, 1, 1, 0));
	}

	if (TargetName)
	{
		TargetName->TextDelegate.BindUFunction(this, "GetTargetName");
	}

	if (LowerBorder)
	{
		UVerticalBoxSlot* LowerBorderBoxSlot = CastChecked<UVerticalBoxSlot>(MainVerticalBox->GetSlots()[1]);

		// Set size to Fill, 1.f
		LowerBorderBoxSlot->SetSize(FSlateChildSize());

		LowerBorderBoxSlot->SetHorizontalAlignment(HAlign_Fill);
		LowerBorderBoxSlot->SetVerticalAlignment(VAlign_Fill);

		LowerBorder->SetHorizontalAlignment(HAlign_Center);
		LowerBorder->SetVerticalAlignment(VAlign_Center);

		LowerBorder->SetBrushColor(FLinearColor(1, 1, 1, 0));
	}

	if (InteractionPrompt)
	{
		InteractionPrompt->TextDelegate.BindUFunction(this, "GetMessageText");
	}
	
	return true;
}

FText URPGInteractionPromptWidget::GetTargetName()
{
	return Text_Target_Name;
}

FText URPGInteractionPromptWidget::GetMessageText()
{
	return Text_Message;
}