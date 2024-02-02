// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/RPGPlayerCharacter.h"
#include "CharacterSelectionScreenOption.generated.h"

class UButton;

/**
 * Character selection screen option that is created in CharacterSelectionScreenWidget
 */
UCLASS()
class INVENTORYPROJECTV3_API UCharacterSelectionScreenOption : public UUserWidget
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonHoveredDelegate, UCharacterSelectionScreenOption*, SelfReference);

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* OptionButton;

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterCustomization")
	FCharacterSelectionData AssociatedCharacterSelectionData;

	FOnButtonHoveredDelegate OnButtonHoveredDelegate;

	/* Bound to OnHovered event and broadcasts OnButtonHoveredDelegate delegate */
	UFUNCTION()
	void OnOptionButtonHovered();

	UFUNCTION()
	void OnOptionsButtonClicked();

	UFUNCTION(BlueprintPure, Category = "CharacterCustomization")
	FText GetButtonText() const;
};
