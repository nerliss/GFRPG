// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGAbilityTooltipWidget.generated.h"

class URPGAbilityBase;
class URPGAbilityDefinitionData;
class UTextBlock;
class UImage;
class URPGAbilityComponent;

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetupTooltip(URPGAbilityDefinitionData* AbilityDefinition);
	
	UFUNCTION()
	void OnAbilityUsed(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityTargetingPreviewStarted(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityTargetingPreviewCanceled(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityTargetingPreviewConfirmed(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityTooltipFadeOut();
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* AbilityIconImage;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* AbilityNameText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* AbilityCooldownText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* AbilityCostText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* AbilityCastRangeText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* AbilityDescriptionText;
	
	UPROPERTY(BlueprintReadOnly)
	URPGAbilityComponent* AbilityComponent;
	
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle VisibilityTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Tooltip")
	float AbilityTooltipVisibilityTime = 3.f;
	
};
