// Oleksandr Tkachov 2021-2026


#include "Widgets/AbilitySystem/RPGCastBarWidget.h"

#include "Abilities/RPGAbilityComponent.h"
#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void URPGCastBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AbilityComponent = Cast<URPGAbilityComponent>(GetOwningPlayerPawn()->GetComponentByClass(URPGAbilityComponent::StaticClass()));
	if (AbilityComponent)
	{
		AbilityComponent->OnAbilityChannelStarted.AddDynamic(this, &URPGCastBarWidget::OnAbilityChannelStarted);
		AbilityComponent->OnAbilityChannelStopped.AddDynamic(this, &URPGCastBarWidget::OnAbilityChannelStopped);
	}
	
	UpdateWidgetVisibility();
}

void URPGCastBarWidget::UpdateWidgetVisibility()
{
	if (!AbilityComponent)
	{
		return;
	}
	
	SetVisibility(AbilityComponent->HasActiveAbilities() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void URPGCastBarWidget::UpdateVisuals()
{
	if (GetVisibility() != ESlateVisibility::Visible)
	{
		return;
	}
	
	if (!CastTimeRemainingTextBlock)
	{
		return;
	}
	
	if (!CastProgressBar)
	{
		return;
	}
	
	if (!ChannelingAbility)
	{
		return;
	}
	
	if (!AbilityComponent)
	{
		return;
	}
	
	if (!ChannelingAbility->GetAbilityDefinition())
	{
		return;
	}
	
	FNumberFormattingOptions NumberFormattingOptions;
	NumberFormattingOptions.MinimumFractionalDigits = 0;
	NumberFormattingOptions.MaximumFractionalDigits = 1;
	
	const float RemainingTime = ChannelingAbility->GetAbilityDefinition()->ActivationMode == EAbilityActivationMode::Channel ? AbilityComponent->GetChannelRemainingTimeForAbility(ChannelingAbility) : AbilityComponent->GetCastRemainingTimeForAbility(ChannelingAbility);
	const FText FormattedText = FText::Format(FText::FromString("{0}"), FText::AsNumber(RemainingTime, &NumberFormattingOptions));
	
	CastTimeRemainingTextBlock->SetText(FormattedText);
	
	const float RemainingProgress = ChannelingAbility->GetAbilityDefinition()->ActivationMode == EAbilityActivationMode::Channel ? AbilityComponent->GetChannelDurationPercentForAbility(ChannelingAbility) : 1 - AbilityComponent->GetCastDurationPercentForAbility(ChannelingAbility);
	
	CastProgressBar->SetPercent(RemainingProgress);
}

void URPGCastBarWidget::OnAbilityChannelStarted(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	const URPGAbilityDefinitionData* AbilityDefinitionData = Ability->GetAbilityDefinition();
	if (!AbilityDefinitionData)
	{
		return;
	}
	
	if (!AbilityIcon)
	{
		return;
	}
	
	if (!AbilityNameTextBlock)
	{
		return;
	}
	
	ChannelingAbility = Ability;
	
	UpdateWidgetVisibility();
	
	AbilityIcon->SetBrushFromTexture(AbilityDefinitionData->Icon);
	AbilityNameTextBlock->SetText(AbilityDefinitionData->Name);
	
	UpdateVisuals();
	
	GetWorld()->GetTimerManager().SetTimer(UpdateVisualsTimerHandle, this, &URPGCastBarWidget::UpdateVisuals, UpdateVisualsRate, true);
}

void URPGCastBarWidget::OnAbilityChannelStopped(URPGAbilityBase* Ability, EAbilityInterruptReason Reason)
{
	UpdateWidgetVisibility();
	
	GetWorld()->GetTimerManager().ClearTimer(UpdateVisualsTimerHandle);
}
