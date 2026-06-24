// Oleksandr Tkachov 2021-2026

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

	ARPGPlayerCharacter* GetPlayerReference() const;

	UPROPERTY()
	ARPGPlayerCharacter* PlayerRef;

	float GetCurrentXP() const;
	float GetMaxXP() const;

protected:
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentLevel() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentPercentageBuffer() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentPercentageXP() const;

	UFUNCTION(BlueprintPure)
	void GetExperience(float& OutCurrentExperience, float& OutMaxExperience) const;
};
