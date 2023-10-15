// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/CharacterCustomizationData.h"
#include "CharacterSelectionScreen_Widget.generated.h"

class UScrollBox;
class UImage;
class UButton;
class UCharacterCustomizationData;
class UCharacterSelectionScreenOption;
class APreviewCharacter;
class UMaterial;

/**
 * Character selection screen widget
 */
UCLASS()
class INVENTORYPROJECTV3_API UCharacterSelectionScreen_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	UCharacterSelectionScreen_Widget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:

	/* Characters list that hosts all available characters options */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* CharactersList;

	/* Preview image of a character */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* CharacterPreview;

	/* Close window button */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCustomization")
	UCharacterCustomizationData* CharacterCustomizationData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterCustomization")
	APreviewCharacter* CharacterPreviewActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCustomization")
	TSubclassOf<APreviewCharacter> CharacterPreviewActorClassToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCustomization")
	UMaterial* CharacterPreviewMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterCustomization")
	TSubclassOf<UCharacterSelectionScreenOption> CharacterSelectionScreenOptionClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	bool bRightMouseButtonDown;

	void SpawnPreviewCharacter();

	UFUNCTION()
	void OnOptionButtonHovered(UCharacterSelectionScreenOption* Option);

	UFUNCTION()
	void CloseWindow();
};
