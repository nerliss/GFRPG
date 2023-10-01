// Destruction Games. 2022


#include "Widgets/RPGXPBar_Widget.h"
#include "Components/ProgressBar.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/RPGXP_Component.h"
#include "Characters/RPGPlayerCharacter.h"

bool URPGXPBar_Widget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return false;
	}

	if (MainOverlay)
	{
		UOverlaySlot* Bar_BufferSlot = CastChecked<UOverlaySlot>(MainOverlay->GetSlots()[0]);
		UOverlaySlot* Bar_XPSlot = CastChecked<UOverlaySlot>(MainOverlay->GetSlots()[1]);
		UOverlaySlot* MainHorizontalBoxSlot = CastChecked<UOverlaySlot>(MainOverlay->GetSlots()[2]);

		Bar_BufferSlot->SetHorizontalAlignment(HAlign_Fill);
		Bar_BufferSlot->SetVerticalAlignment(VAlign_Fill);

		Bar_XPSlot->SetHorizontalAlignment(HAlign_Fill);
		Bar_XPSlot->SetVerticalAlignment(VAlign_Fill);

		MainHorizontalBoxSlot->SetHorizontalAlignment(HAlign_Fill);
		MainHorizontalBoxSlot->SetVerticalAlignment(VAlign_Center);
	}

	if (Bar_Buffer)
	{
		Bar_Buffer->SetFillColorAndOpacity(FLinearColor(0.79f, 0.f, 0.6f, 1.f));
	}

	if (Bar_XP)
	{
		Bar_XP->SetFillColorAndOpacity(FLinearColor(0.17f, 0.f, 0.13f, 1.f));
	}

	if (MainHorizontalBox)
	{
		UHorizontalBoxSlot* LevelTextSlot = CastChecked<UHorizontalBoxSlot>(MainHorizontalBox->GetSlots()[0]);
		UHorizontalBoxSlot* ExperienceHorizontalBoxSlot = CastChecked<UHorizontalBoxSlot>(MainHorizontalBox->GetSlots()[1]);

		LevelTextSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		LevelTextSlot->SetHorizontalAlignment(HAlign_Fill);
		LevelTextSlot->SetVerticalAlignment(VAlign_Fill);

		ExperienceHorizontalBoxSlot->SetSize(FSlateChildSize());
		ExperienceHorizontalBoxSlot->SetHorizontalAlignment(HAlign_Center);
		ExperienceHorizontalBoxSlot->SetVerticalAlignment(VAlign_Center);
	}
	
	return true;
}

void URPGXPBar_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	// Set reference to player
	PlayerRef = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
}

const ARPGPlayerCharacter* URPGXPBar_Widget::GetPlayerReference()
{
	return PlayerRef ? PlayerRef : nullptr;
}

const float URPGXPBar_Widget::GetCurrentXP()
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetXP_Current() : -1.f;
}

const float URPGXPBar_Widget::GetMaxXP()
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetXP_Current_Max() : -1.f;
}

const int32 URPGXPBar_Widget::GetCurrentLevel()
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetLevelCurrent() : -1;
}

const float URPGXPBar_Widget::GetCurrentPercentageBuffer()
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentPercentage_Buffer() : -1.f;
}

const float URPGXPBar_Widget::GetCurrentPercentageXP()
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentPercentage_XP() : -1.f;
}

const void URPGXPBar_Widget::GetExperience(float& OutCurrentExperience, float& OutMaxExperience)
{
	OutCurrentExperience = GetCurrentXP();
	OutMaxExperience = GetMaxXP();
}
