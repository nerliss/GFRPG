// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGAbilityTooltip.generated.h"

class UTextBlock;
class UImage;
class URPGAbilityComponent;

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGAbilityTooltip : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
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
	
};
