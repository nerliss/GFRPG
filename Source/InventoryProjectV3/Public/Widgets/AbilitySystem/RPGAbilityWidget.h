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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseHoverStart, URPGAbilityDefinitionData*, DefinitionData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseHoverEnd, URPGAbilityDefinitionData*, DefinitionData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRightMouseButtonClicked, URPGAbilityDefinitionData*, DefinitionData);

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
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
	
	// TODO: Change back to Read only after spell book is done
	UPROPERTY(BlueprintReadWrite)
	URPGAbilityDefinitionData* AbilityDefinition;
	
	UPROPERTY(BlueprintReadOnly)
	URPGAbilityComponent* AbilityComponent;

	// TODO: Change back to Read only after spell book is done
	UPROPERTY(BlueprintReadWrite)
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
	
	FOnMouseHoverStart OnMouseHoverStart;
	FOnMouseHoverEnd OnMouseHoverEnd;
	FOnRightMouseButtonClicked OnRightMouseButtonClicked;
	
private:
	
	UFUNCTION()
	void OnCooldownAnimationFinished();
};
