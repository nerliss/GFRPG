// Destruction Games. 2022

#include "Widgets/RPGInteractionPrompt_Widget.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void URPGInteractionPrompt_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool URPGInteractionPrompt_Widget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return false;
	}

	// Set default values for bind variables
	Text_Message = FText::FromString(TEXT("Press E to interact"));
	Text_Target_Name = FText::FromString(TEXT("Target name"));

	if (MainSizeBox)
	{
		MainSizeBox->SetMinDesiredHeight(100.f);
		MainSizeBox->SetMinDesiredWidth(300.f);
	}

	if (UpperBorder)
	{
		// Get reference to first slot of the vertical box
		// (Use of CastChecked implies that the object it returns is of a specific type, does not check for nullptrs)
		UVerticalBoxSlot* UpperBorderBoxSlot = CastChecked<UVerticalBoxSlot>(MainVerticalBox->GetSlots()[0]);
		
		// Set size to fill, 1.f
		UpperBorderBoxSlot->SetSize(FSlateChildSize());
		
		// Set slot alignment
		UpperBorderBoxSlot->SetHorizontalAlignment(HAlign_Fill);
		UpperBorderBoxSlot->SetVerticalAlignment(VAlign_Fill);

		// Set content alignment
		UpperBorder->SetHorizontalAlignment(HAlign_Center);
		UpperBorder->SetVerticalAlignment(VAlign_Center);

		// Set border color
		UpperBorder->SetBrushColor(FLinearColor(1, 1, 1, 0));
	}

	if (TargetName)
	{
		// Bind function
		TargetName->TextDelegate.BindUFunction(this, "GetTargetName");
	}

	if (LowerBorder)
	{
		// Get reference to first slot of the vertical box 
		// (Use of CastChecked implies that the object it returns is of a specific type, does not check for nullptrs)
		UVerticalBoxSlot* LowerBorderBoxSlot = CastChecked<UVerticalBoxSlot>(MainVerticalBox->GetSlots()[1]);

		// Set size to Fill, 1.f
		LowerBorderBoxSlot->SetSize(FSlateChildSize());

		// Set slot alignment
		LowerBorderBoxSlot->SetHorizontalAlignment(HAlign_Fill);
		LowerBorderBoxSlot->SetVerticalAlignment(VAlign_Fill);

		// Set content alignment
		LowerBorder->SetHorizontalAlignment(HAlign_Center);
		LowerBorder->SetVerticalAlignment(VAlign_Center);

		// Set border color
		LowerBorder->SetBrushColor(FLinearColor(1, 1, 1, 0));
	}

	if (InteractionPrompt)
	{
		// Bind function
		InteractionPrompt->TextDelegate.BindUFunction(this, "GetMessageText");
	}
	
	return true;
}

FText URPGInteractionPrompt_Widget::GetTargetName()
{
	return Text_Target_Name;
}

FText URPGInteractionPrompt_Widget::GetMessageText()
{
	return Text_Message;
}