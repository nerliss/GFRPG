// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGSpellbookWidget.generated.h"

class URPGAbilityWidget;
class URPGAbilityTooltip;
class UTextBlock;
class UButton;
class UUniformGridPanel;
class URPGAbilityBase;
class URPGAbilityComponent;
class URPGAbilityDefinitionData;
/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGSpellbookWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void LoadAllAbilitiesFromRegistry();
	
	UFUNCTION(BlueprintCallable)
	void PopulatePage();
	
	UFUNCTION(BlueprintCallable)
	void UpdateButtons();
	
	UFUNCTION()
	void OnNextPageButtonClicked();
	
	UFUNCTION()
	void OnPreviousPageButtonClicked();
	
	UFUNCTION()
	void OnCloseButtonClicked();
	
	UFUNCTION()
	void OnAbilityMouseOverStart(URPGAbilityDefinitionData* AbilityDefinition);
	
	UFUNCTION()
	void OnAbilityMouseOverEnd(URPGAbilityDefinitionData* AbilityDefinition);
	
	UFUNCTION()
	void OnAbilityRightMouseButtonClicked(URPGAbilityDefinitionData* AbilityDefinition);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UUniformGridPanel* SpellBookUniformGrid;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* NextPageButton;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* PreviousPageButton;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* CloseButton;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* PageNumberText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	URPGAbilityTooltip* AbilityTooltipWidget;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<URPGAbilityDefinitionData*> AllAbilitiesInGame;
	
	UPROPERTY(BlueprintReadOnly)
	URPGAbilityComponent* AbilityComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<URPGAbilityWidget> AbilityWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	int32 AbilitiesPerPage = 9;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	int32 GridColumns = 3;
	
	// TODO: Remove
	UPROPERTY(BlueprintReadOnly)
	TMap<URPGAbilityDefinitionData*, URPGAbilityBase*> Abilities;
	
private:
	
	int32 CurrentPage;
	int32 TotalPages;
	int32 TotalAbilities;
};
