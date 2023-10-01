// Destruction Games. 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGXPBar_Widget.generated.h"

class UProgressBar;
class UOverlay;
class UTextBlock;
class UHorizontalBox;
class ARPGPlayerCharacter;

UCLASS()
class INVENTORYPROJECTV3_API URPGXPBar_Widget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	bool Initialize() override;

public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* MainOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* Bar_Buffer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* Bar_XP;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* MainHorizontalBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LevelText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* XPText;

private:

	const ARPGPlayerCharacter* GetPlayerReference();

	UPROPERTY()
	ARPGPlayerCharacter* PlayerRef;

	const float GetCurrentXP();
	const float GetMaxXP();

protected:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const int32 GetCurrentLevel();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetCurrentPercentageBuffer();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetCurrentPercentageXP();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const void GetExperience(float& OutCurrentExperience, float& OutMaxExperience);
};
