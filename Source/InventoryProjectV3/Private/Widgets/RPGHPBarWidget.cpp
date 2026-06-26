// Oleksandr Tkachov 2021-2026


#include "Widgets/RPGHPBarWidget.h"
#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGHealthComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

bool URPGHPBarWidget::Initialize()
{
	if (!Super::Initialize())
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

void URPGHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerRef = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
}

const ARPGPlayerCharacter* URPGHPBarWidget::GetPlayerReference()
{
	return PlayerRef;
}

const float URPGHPBarWidget::GetCurrentHP()
{
	return GetPlayerReference() ? PlayerRef->GetHealthComponent()->GetCurrentHealth() : -1.f;
}

const float URPGHPBarWidget::GetMaxHP()
{
	return GetPlayerReference() ? PlayerRef->GetHealthComponent()->GetMaxHealth() : -1.f;
}

const float URPGHPBarWidget::GetCurrentHPPercent()
{
	return GetCurrentHP() / GetMaxHP();
}

const void URPGHPBarWidget::GetHealth(float& OutCurrentHealth, float& OutMaxHealth)
{
	OutCurrentHealth = GetCurrentHP();
	OutMaxHealth = GetMaxHP();
}
