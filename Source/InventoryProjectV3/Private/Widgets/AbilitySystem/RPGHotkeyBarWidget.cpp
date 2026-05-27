// Oleksandr Tkachov 2021-2026


#include "Widgets/AbilitySystem/RPGHotkeyBarWidget.h"

#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "Components/GridPanel.h"
#include "Components/Overlay.h"
#include "Widgets/AbilitySystem/RPGAbilityWidget.h"

void URPGHotkeyBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AbilityComponent = GetOwningPlayerPawn()->GetComponentByClass<URPGAbilityComponent>();
	if (AbilityComponent)
	{
		AbilityDefinitions = AbilityComponent->SpawnedAbilityDefinitions;
		FillActionBar();
		
		AbilityComponent->OnAbilityUsed.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityUsed);
		AbilityComponent->OnAbilityAdded.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityAdded);
		AbilityComponent->OnAbilityCooldownStarted.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityCooldownStarted);
		AbilityComponent->OnAbilityCooldownEnded.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityCooldownEnded);
		AbilityComponent->OnAbilityToggled.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityToggled);
		AbilityComponent->OnAbilityTargetingPreviewStarted.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityTargetingPreviewStarted);
		AbilityComponent->OnAbilityTargetingPreviewConfirmed.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityTargetingPreviewConfirmed);
		AbilityComponent->OnAbilityTargetingPreviewCanceled.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityTargetingPreviewCanceled);
		AbilityComponent->OnAbilityChannelStarted.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityChannelStarted);
		AbilityComponent->OnAbilityChannelStopped.AddDynamic(this, &URPGHotkeyBarWidget::OnAbilityChannelStopped);
	}
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
	if (!Ability)
	{
		return;
	}
	
	URPGAbilityWidget* AbilityWidget = GetAbilityWidget(Ability);
	if (!AbilityWidget)
	{
		return;
	}
	
	if (!AbilityWidget->AbilityUsed)
	{
		return;
	}
	
	AbilityWidget->PlayAnimation(AbilityWidget->AbilityUsed, 0.f, 1, EUMGSequencePlayMode::PingPong, 10.f, false);
}

void URPGHotkeyBarWidget::OnAbilityAdded(URPGAbilityDefinitionData* AbilityDefinition)
{
	// Not used for now
	if (!AbilityDefinition)
	{
		return;
	}
	
	if (!AbilityComponent)
	{
		return;
	}
	
	AbilityDefinitions = AbilityComponent->SpawnedAbilityDefinitions;
	FillActionBar();
}

void URPGHotkeyBarWidget::OnAbilityCooldownStarted(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	if (!AbilityComponent)
	{
		return;
	}
	
	URPGAbilityWidget* AbilityWidget = GetAbilityWidget(Ability);
	if (!AbilityWidget)
	{
		return;
	}
	
	AbilityWidget->StartCooldownVisual(AbilityComponent);
}

void URPGHotkeyBarWidget::OnAbilityCooldownEnded(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	if (!AbilityComponent)
	{
		return;
	}
	
	URPGAbilityWidget* AbilityWidget = GetAbilityWidget(Ability);
	if (!AbilityWidget)
	{
		return;
	}
	
	AbilityWidget->StopCooldownVisual();
}

void URPGHotkeyBarWidget::OnAbilityToggled(URPGAbilityBase* Ability, bool bToggled)
{
	if (!Ability)
	{
		return;
	}
	
	if (!AbilityComponent)
	{
		return;
	}
	
	const URPGAbilityWidget* AbilityWidget = GetAbilityWidget(Ability);
	if (!AbilityWidget)
	{
		return;
	}

	if (!AbilityWidget->ToggleOverlay)
	{
		return;
	}
	
	// TODO: check usage of bToggled param since original BP logic uses Ability's IsToggled
	AbilityWidget->ToggleOverlay->SetVisibility(bToggled ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void URPGHotkeyBarWidget::OnAbilityTargetingPreviewStarted(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	URPGAbilityWidget* AbilityWidget = GetAbilityWidget(Ability);
	if (!AbilityWidget)
	{
		return;
	}
	
	AbilityWidget->UpdateBackgroundBorder(true);
}

void URPGHotkeyBarWidget::OnAbilityTargetingPreviewConfirmed(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	URPGAbilityWidget* AbilityWidget = GetAbilityWidget(Ability);
	if (!AbilityWidget)
	{
		return;
	}
	
	AbilityWidget->UpdateBackgroundBorder(false);
}

void URPGHotkeyBarWidget::OnAbilityTargetingPreviewCanceled(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	URPGAbilityWidget* AbilityWidget = GetAbilityWidget(Ability);
	if (!AbilityWidget)
	{
		return;
	}
	
	AbilityWidget->UpdateBackgroundBorder(false);
}

void URPGHotkeyBarWidget::OnAbilityChannelStarted(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	URPGAbilityWidget* AbilityWidget = GetAbilityWidget(Ability);
	if (!AbilityWidget)
	{
		return;
	}
	
	AbilityWidget->UpdateBackgroundBorder(true);
}

void URPGHotkeyBarWidget::OnAbilityChannelStopped(URPGAbilityBase* Ability, EAbilityInterruptReason Reason)
{
	if (!Ability)
	{
		return;
	}
	
	URPGAbilityWidget* AbilityWidget = GetAbilityWidget(Ability);
	if (!AbilityWidget)
	{
		return;
	}
	
	AbilityWidget->UpdateBackgroundBorder(false);
}
