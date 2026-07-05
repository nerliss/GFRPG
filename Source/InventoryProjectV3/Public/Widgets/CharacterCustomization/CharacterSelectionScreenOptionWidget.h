// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/RPGPlayerCharacter.h"
#include "CharacterSelectionScreenOptionWidget.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonHoveredDelegate, UCharacterSelectionScreenOptionWidget*, SelfReference);

/**
 * Character selection screen option that is created in CharacterSelectionScreenWidget
 */
UCLASS()
class INVENTORYPROJECTV3_API UCharacterSelectionScreenOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/* Bound to OnHovered event and broadcasts OnButtonHoveredDelegate delegate */
	UFUNCTION()
	void OnOptionButtonHovered();

	UFUNCTION()
	void OnOptionsButtonClicked();

	UFUNCTION(BlueprintPure, Category = "CharacterCustomization")
	FText GetButtonText() const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterCustomization")
	FCharacterSelectionData AssociatedCharacterSelectionData;

	FOnButtonHoveredDelegate OnButtonHoveredDelegate;

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* OptionButton;
	
};
