// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGHPBarWidget.generated.h"

class UProgressBar;
class ARPGPlayerCharacter;
class UTextBlock;
class UOverlay;
class UHorizontalBox;

UCLASS()
class INVENTORYPROJECTV3_API URPGHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	bool Initialize() override;

public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* MainOverlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* HPText;
	
private:

	const ARPGPlayerCharacter* GetPlayerReference();

	UPROPERTY()
	ARPGPlayerCharacter* PlayerRef;

	const float GetCurrentHP();
	const float GetMaxHP();

protected:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetCurrentHPPercent();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const void GetHealth(float& OutCurrentHealth, float& OutMaxHealth);
};
