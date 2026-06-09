// Oleksandr Tkachov 2021-2026


#include "Widgets/AbilitySystem/RPGAbilityTooltip.h"

#include "Abilities/RPGAbilityComponent.h"
#include "Abilities/RPGAbilityDefinitionData.h"
#include "Abilities/Archetypes/RPGAbilityBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URPGAbilityTooltip::NativeConstruct()
{
	Super::NativeConstruct();
	
	AbilityComponent = Cast<URPGAbilityComponent>(GetOwningPlayerPawn()->GetComponentByClass(URPGAbilityComponent::StaticClass()));
	if (AbilityComponent)
	{
		AbilityComponent->OnAbilityUsed.AddDynamic(this, &URPGAbilityTooltip::OnAbilityUsed);
		AbilityComponent->OnAbilityTargetingPreviewStarted.AddDynamic(this, &URPGAbilityTooltip::OnAbilityTargetingPreviewStarted);
		AbilityComponent->OnAbilityTargetingPreviewCanceled.AddDynamic(this, &URPGAbilityTooltip::OnAbilityTargetingPreviewCanceled);
		AbilityComponent->OnAbilityTargetingPreviewConfirmed.AddDynamic(this, &URPGAbilityTooltip::OnAbilityTargetingPreviewConfirmed);
	}
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void URPGAbilityTooltip::SetupTooltip(URPGAbilityDefinitionData* AbilityDefinition)
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

void URPGAbilityTooltip::OnAbilityUsed(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	SetupTooltip(Ability->GetAbilityDefinition());
	
	GetWorld()->GetTimerManager().SetTimer(VisibilityTimerHandle, this, &URPGAbilityTooltip::OnAbilityTooltipFadeOut, AbilityTooltipVisibilityTime);
}

void URPGAbilityTooltip::OnAbilityTargetingPreviewStarted(URPGAbilityBase* Ability)
{
	if (!Ability)
	{
		return;
	}
	
	SetupTooltip(Ability->GetAbilityDefinition());
	
	GetWorld()->GetTimerManager().ClearTimer(VisibilityTimerHandle);
}

void URPGAbilityTooltip::OnAbilityTargetingPreviewCanceled(URPGAbilityBase* Ability)
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void URPGAbilityTooltip::OnAbilityTargetingPreviewConfirmed(URPGAbilityBase* Ability)
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void URPGAbilityTooltip::OnAbilityTooltipFadeOut()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
