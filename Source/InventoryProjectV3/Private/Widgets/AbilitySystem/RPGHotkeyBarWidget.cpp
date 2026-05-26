// Oleksandr Tkachov 2021-2026


#include "Widgets/AbilitySystem/RPGHotkeyBarWidget.h"

#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "Components/GridPanel.h"

void URPGHotkeyBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply URPGHotkeyBarWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}

void URPGHotkeyBarWidget::FillActionBar()
{
	if (!ActionBarGridPanel)
	{
		return;
	}
	
	ActionBarGridPanel->ClearChildren();
	// TODO: continue
}

URPGAbilityWidget* URPGHotkeyBarWidget::GetAbilityWidget(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return nullptr;
	}
	
	if (!Ability->GetAbilityDefinition())
	{
		return nullptr;
	}
	
	URPGAbilityWidget** AbilityDefinitionSlot = AbilityDefinitionsSlots.Find(Ability->GetAbilityDefinition());
	return AbilityDefinitionSlot ? *AbilityDefinitionSlot : nullptr;
}

void URPGHotkeyBarWidget::OnAbilityUsed(URPGAbilityBase* Ability)
{
}

void URPGHotkeyBarWidget::OnAbilityAdded(URPGAbilityDefinitionData* AbilityDefinition)
{
}

void URPGHotkeyBarWidget::OnAbilityCooldownStarted(URPGAbilityBase* Ability)
{
}

void URPGHotkeyBarWidget::OnAbilityCooldownEnded(URPGAbilityBase* Ability)
{
}

void URPGHotkeyBarWidget::OnAbilityToggled(URPGAbilityBase* Ability, bool bToggled)
{
}

void URPGHotkeyBarWidget::OnAbilityTargetingPreviewStarted(URPGAbilityBase* Ability)
{
}

void URPGHotkeyBarWidget::OnAbilityTargetingPreviewConfirmed(URPGAbilityBase* Ability)
{
}

void URPGHotkeyBarWidget::OnAbilityTargetingPreviewCanceled(URPGAbilityBase* Ability)
{
}

void URPGHotkeyBarWidget::OnAbilityChannelStarted(URPGAbilityBase* Ability)
{
}

void URPGHotkeyBarWidget::OnAbilityChannelStopped(URPGAbilityBase* Ability, EAbilityInterruptReason Reason)
{
}
