// Oleksandr Tkachov 2021-2026


#include "Widgets/AbilitySystem/RPGAbilityTooltipWidget.h"

#include "Abilities/RPGAbilityComponent.h"
#include "Abilities/RPGAbilityDefinitionData.h"
#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URPGAbilityTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AbilityComponent = Cast<URPGAbilityComponent>(GetOwningPlayerPawn()->GetComponentByClass(URPGAbilityComponent::StaticClass()));
	if (AbilityComponent)
	{
		AbilityComponent->OnAbilityUsed.AddDynamic(this, &URPGAbilityTooltipWidget::OnAbilityUsed);
		AbilityComponent->OnAbilityTargetingPreviewStarted.AddDynamic(this, &URPGAbilityTooltipWidget::OnAbilityTargetingPreviewStarted);
		AbilityComponent->OnAbilityTargetingPreviewCanceled.AddDynamic(this, &URPGAbilityTooltipWidget::OnAbilityTargetingPreviewCanceled);
		AbilityComponent->OnAbilityTargetingPreviewConfirmed.AddDynamic(this, &URPGAbilityTooltipWidget::OnAbilityTargetingPreviewConfirmed);
	}
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void URPGAbilityTooltipWidget::SetupTooltip(URPGAbilityDefinitionData* AbilityDefinition)
{
	if (!AbilityDefinition)
	{
		return;
	}
	
	if (!AbilityNameText)
	{
		return;
	}
	
	if (!AbilityDescriptionText)
	{
		return;
	}
	
	if (!AbilityIconImage)
	{
		return;
	}
	
	if (!AbilityDescriptionText)
	{
		return;
	}
	
	if (!AbilityCostText)
	{
		return;
	}
	
	if (!AbilityCooldownText)
	{
		return;
	}
	
	if (!AbilityCastRangeText)
	{
		return;
	}
	
	SetVisibility(ESlateVisibility::Visible);
	
	AbilityNameText->SetText(AbilityDefinition->Name);
	AbilityIconImage->SetBrushFromTexture(AbilityDefinition->Icon);
	AbilityDescriptionText->SetText(AbilityDefinition->Description);
	
	const FText CostText = FText::Format(FText::FromString("Cost: {0}"), FText::AsNumber(AbilityDefinition->Cost));
	AbilityCostText->SetText(CostText);
	
	const FText CooldownText = FText::Format(FText::FromString("Cooldown: {0} sec"), FText::AsNumber(AbilityDefinition->Cooldown));
	AbilityCooldownText->SetText(CooldownText);
	
	const FText CastRangeText = FText::Format(FText::FromString("Cast Range: {0}"), FText::AsNumber(AbilityDefinition->CastRange));
	AbilityCastRangeText->SetText(CastRangeText);
}

void URPGAbilityTooltipWidget::OnAbilityUsed(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	SetupTooltip(Ability->GetAbilityDefinition());
	
	GetWorld()->GetTimerManager().SetTimer(VisibilityTimerHandle, this, &URPGAbilityTooltipWidget::OnAbilityTooltipFadeOut, AbilityTooltipVisibilityTime);
}

void URPGAbilityTooltipWidget::OnAbilityTargetingPreviewStarted(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	SetupTooltip(Ability->GetAbilityDefinition());
	
	GetWorld()->GetTimerManager().ClearTimer(VisibilityTimerHandle);
}

void URPGAbilityTooltipWidget::OnAbilityTargetingPreviewCanceled(URPGAbilityBase* Ability)
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void URPGAbilityTooltipWidget::OnAbilityTargetingPreviewConfirmed(URPGAbilityBase* Ability)
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void URPGAbilityTooltipWidget::OnAbilityTooltipFadeOut()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
