// Destruction Games. 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/CharacterCustomizationData.h"
#include "CharacterSelectionScreen_Widget.generated.h"

class UScrollBox;
class UImage;
class UButton;
class UCharacterCustomizationData;
class APreviewCharacter;

/**
 * Character selection screen widget
 */
UCLASS()
class INVENTORYPROJECTV3_API UCharacterSelectionScreen_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	UCharacterSelectionScreen_Widget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	bool bRightMouseButtonDown;

	// MyTODO: Move all the logic from blueprint here

};
