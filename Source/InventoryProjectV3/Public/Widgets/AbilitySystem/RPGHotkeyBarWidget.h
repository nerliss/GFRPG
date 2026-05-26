// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGHotkeyBarWidget.generated.h"

enum class EAbilityInterruptReason : uint8;
class URPGAbilityDefinitionData;
class URPGAbilityWidget;
class URPGAbilityBase;
class UGridPanel;
class URPGAbilityComponent;
/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGHotkeyBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	
	void FillActionBar();
	URPGAbilityWidget* GetAbilityWidget(URPGAbilityBase* Ability); 
	
	UFUNCTION()
	void OnAbilityUsed(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityAdded(URPGAbilityDefinitionData* AbilityDefinition);
	
	UFUNCTION()
	void OnAbilityCooldownStarted(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityCooldownEnded(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityToggled(URPGAbilityBase* Ability, bool bToggled);
	
	UFUNCTION()
	void OnAbilityTargetingPreviewStarted(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityTargetingPreviewConfirmed(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityTargetingPreviewCanceled(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityChannelStarted(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityChannelStopped(URPGAbilityBase* Ability, EAbilityInterruptReason Reason);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UGridPanel* ActionBarGridPanel;
	
	UPROPERTY(BlueprintReadOnly)
	URPGAbilityComponent* AbilityComponent;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<URPGAbilityDefinitionData*> AbilityDefinitions;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<URPGAbilityDefinitionData*, URPGAbilityWidget*> AbilityDefinitionsSlots;
	
};
