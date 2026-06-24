// Oleksandr Tkachov 2021-2026


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
	if (!Super::Initialize())
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

	PlayerRef = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
}

ARPGPlayerCharacter* URPGXPBar_Widget::GetPlayerReference() const
{
	return PlayerRef;
}

float URPGXPBar_Widget::GetCurrentXP() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentXP() : -1.f;
}

float URPGXPBar_Widget::GetMaxXP() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentMaxXP() : -1.f;
}

int32 URPGXPBar_Widget::GetCurrentLevel() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentLevel() : -1;
}

float URPGXPBar_Widget::GetCurrentPercentageBuffer() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentPercentageBuffer() : -1.f;
}

float URPGXPBar_Widget::GetCurrentPercentageXP() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentXPPercentage() : -1.f;
}

void URPGXPBar_Widget::GetExperience(float& OutCurrentExperience, float& OutMaxExperience) const
{
	OutCurrentExperience = GetCurrentXP();
	OutMaxExperience = GetMaxXP();
}
