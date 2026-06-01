// Oleksandr Tkachov 2021-2026


#include "Widgets/AbilitySystem/RPGAbilityWidget.h"

#include "Abilities/RPGAbilityComponent.h"
#include "Abilities/RPGAbilityDefinitionData.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void URPGAbilityWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (AbilityDefinition)
	{
		if (AbilityThumbnailImage)
		{
			AbilityThumbnailImage->SetBrushFromTexture(AbilityDefinition->Icon);
		}
	}
	else
	{
		if (AbilityThumbnailImage)
		{
			AbilityThumbnailImage->SetBrush(EmptyAbilitySlotBrush);
		}
	}
}

void URPGAbilityWidget::UpdateCooldownVisual()
{
	if (!CooldownText)
	{
		return;
	}
	
	if (!CooldownProgressBar)
	{
		return;
	}
	
	if (!Ability)
	{
		return;
	}
	
	if (!AbilityComponent)
	{
		return;
	}
	
	const float CooldownRemaining = AbilityComponent->GetCooldownRemainingForAbility(Ability);
	const float CooldownPercent = AbilityComponent->GetCooldownPercentForAbility(Ability);
	const int32 FractionalDigits = (CooldownRemaining <= 3.f) ? 1 : 0;
	
	FNumberFormattingOptions NumberFormattingOptions;
	NumberFormattingOptions.MinimumFractionalDigits = FractionalDigits;
	NumberFormattingOptions.MaximumFractionalDigits = FractionalDigits;
	
	const FText Text = FText::AsNumber(CooldownRemaining, &NumberFormattingOptions);
	CooldownText->SetText(Text);
	
	CooldownProgressBar->SetPercent(CooldownPercent);
}

void URPGAbilityWidget::StartCooldownVisual(URPGAbilityComponent* InAbilityComponent)
{
	if (!CooldownOverlay)
	{
		return;
	}
	
	AbilityComponent = InAbilityComponent;
	
	CooldownOverlay->SetVisibility(ESlateVisibility::Visible);
	
	UpdateCooldownVisual();
	
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &URPGAbilityWidget::UpdateCooldownVisual, VisualsUpdateRate, true, VisualsUpdateRate);
}

void URPGAbilityWidget::StopCooldownVisual()
{
	if (!CooldownFinished)
	{
		OnCooldownAnimationFinished();
		return;
	}
	
	FWidgetAnimationDynamicEvent CooldownFinishedAnimationDelegate;
	CooldownFinishedAnimationDelegate.BindDynamic(this, &URPGAbilityWidget::OnCooldownAnimationFinished);
	
	UnbindAllFromAnimationFinished(CooldownFinished);
	BindToAnimationFinished(CooldownFinished, CooldownFinishedAnimationDelegate);
	
	PlayAnimation(CooldownFinished, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
}

void URPGAbilityWidget::UpdateBackgroundBorder(const bool bAbilityInUse)
{
	if (!BackgroundBorder)
	{
		return;
	}
	
	BackgroundBorder->SetBrushColor(bAbilityInUse ? BackgroundBorderColorInUse : BackgroundBorderColorDefault);
}

void URPGAbilityWidget::OnCooldownAnimationFinished()
{
	if (!CooldownOverlay)
	{
		return;
	}
	
	if (!AbilityComponent)
	{
		return;
	}
	
	if (!Ability)
	{
		return;
	}
	
	if (AbilityComponent->IsAbilityOnCooldown(Ability))
	{
		return;
	}
	
	CooldownOverlay->SetRenderScale(FVector2D(1.f));
	CooldownOverlay->SetVisibility(ESlateVisibility::Collapsed);
}
