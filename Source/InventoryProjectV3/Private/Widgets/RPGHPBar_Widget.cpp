// Oleksandr Tkachov 2022-2024


#include "Widgets/RPGHPBar_Widget.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGHealth_Component.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

bool URPGHPBar_Widget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success)
	{
		return false;
	}

	if (MainOverlay)
	{
		UOverlaySlot* HPBarSlot = CastChecked<UOverlaySlot>(MainOverlay->GetSlots()[0]);
		UOverlaySlot* HPTextSlot = CastChecked<UOverlaySlot>(MainOverlay->GetSlots()[1]);

		HPBarSlot->SetHorizontalAlignment(HAlign_Fill);
		HPBarSlot->SetVerticalAlignment(VAlign_Fill);

		HPTextSlot->SetHorizontalAlignment(HAlign_Center);
		HPTextSlot->SetVerticalAlignment(VAlign_Center);
	}

	if (HPBar)
	{
		HPBar->SetFillColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}

	return true;
}

void URPGHPBar_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Set player reference
	PlayerRef = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
}

const ARPGPlayerCharacter* URPGHPBar_Widget::GetPlayerReference()
{
	return PlayerRef ? PlayerRef : nullptr;
}

const float URPGHPBar_Widget::GetCurrentHP()
{
	return GetPlayerReference() ? PlayerRef->HPComp->GetCurrentHealth() : -1.f;
}

const float URPGHPBar_Widget::GetMaxHP()
{
	return GetPlayerReference() ? PlayerRef->HPComp->GetMaxHealth() : -1.f;
}

const float URPGHPBar_Widget::GetCurrentHPPercent()
{
	return GetCurrentHP() / GetMaxHP();
}

const void URPGHPBar_Widget::GetHealth(float& OutCurrentHealth, float& OutMaxHealth)
{
	OutCurrentHealth = GetCurrentHP();
	OutMaxHealth = GetMaxHP();
}
