// Oleksandr Tkachov 2021-2026


#include "Widgets/RPGXPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/RPGXPComponent.h"
#include "Characters/RPGPlayerCharacter.h"

bool URPGXPBarWidget::Initialize()
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

void URPGXPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerRef = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
}

ARPGPlayerCharacter* URPGXPBarWidget::GetPlayerReference() const
{
	return PlayerRef;
}

float URPGXPBarWidget::GetCurrentXP() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentXP() : -1.f;
}

float URPGXPBarWidget::GetMaxXP() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentMaxXP() : -1.f;
}

int32 URPGXPBarWidget::GetCurrentLevel() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentLevel() : -1;
}

float URPGXPBarWidget::GetCurrentPercentageBuffer() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentPercentageBuffer() : -1.f;
}

float URPGXPBarWidget::GetCurrentPercentageXP() const
{
	return GetPlayerReference() ? PlayerRef->XPComp->GetCurrentXPPercentage() : -1.f;
}

void URPGXPBarWidget::GetExperience(float& OutCurrentExperience, float& OutMaxExperience) const
{
	OutCurrentExperience = GetCurrentXP();
	OutMaxExperience = GetMaxXP();
}
