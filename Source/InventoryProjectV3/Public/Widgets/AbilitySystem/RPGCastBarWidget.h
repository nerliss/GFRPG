// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGCastBarWidget.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
class URPGAbilityComponent;
enum class EAbilityInterruptReason : uint8;
class URPGAbilityBase;
/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGCastBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
	void UpdateWidgetVisibility();
	void UpdateVisuals();
	
	UFUNCTION()
	void OnAbilityChannelStarted(URPGAbilityBase* Ability);
	
	UFUNCTION()
	void OnAbilityChannelStopped(URPGAbilityBase* Ability, EAbilityInterruptReason Reason);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* CastTimeRemainingTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* AbilityNameTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* AbilityIcon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UProgressBar* CastProgressBar;
	
	UPROPERTY(BlueprintReadOnly)
	URPGAbilityComponent* AbilityComponent;
	
	UPROPERTY(BlueprintReadOnly)
	URPGAbilityBase* ChannelingAbility;
	
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle UpdateVisualsTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Channel")
	float UpdateVisualsRate = 0.05f;
};
