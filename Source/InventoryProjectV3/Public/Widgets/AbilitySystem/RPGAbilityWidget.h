// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGAbilityWidget.generated.h"

class UProgressBar;
class URPGAbilityBase;
class UImage;
class UOverlay;
class UTextBlock;
class UBorder;
class URPGAbilityDefinitionData;
class URPGAbilityComponent;

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
	void UpdateCooldownVisual();
	void StartCooldownVisual(URPGAbilityComponent* InAbilityComponent);
	void StopCooldownVisual();
	void UpdateBackgroundBorder(const bool bAbilityInUse);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* AbilityThumbnailImage;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UBorder* BackgroundBorder;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UOverlay* CooldownOverlay;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* CooldownText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UProgressBar* CooldownProgressBar;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UOverlay* ToggleOverlay;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* HotkeyText;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CooldownFinished;
	
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* AbilityUsed;
	
	UPROPERTY(BlueprintReadOnly)
	URPGAbilityDefinitionData* AbilityDefinition;
	
	UPROPERTY(BlueprintReadOnly)
	URPGAbilityComponent* AbilityComponent;

	UPROPERTY(BlueprintReadOnly)
	URPGAbilityBase* Ability;
	
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle CooldownTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FSlateBrush EmptyAbilitySlotBrush;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FLinearColor BackgroundBorderColorInUse = FLinearColor::Yellow;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FLinearColor BackgroundBorderColorDefault = FLinearColor::Gray;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float VisualsUpdateRate = 0.1f;
	
private:
	
	UFUNCTION()
	void OnCooldownAnimationFinished();
};
