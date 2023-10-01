// Destruction Games. 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGInteractionPrompt_Widget.generated.h"

class USizeBox;
class UVerticalBox;
class UBorder;
class UTextBlock;

UCLASS()
class INVENTORYPROJECTV3_API URPGInteractionPrompt_Widget : public UUserWidget
{
	GENERATED_BODY()

private:

	// Is used instead of NativePreConstruct()
	bool Initialize() override;

public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USizeBox* MainSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* MainVerticalBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* UpperBorder = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TargetName = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* LowerBorder = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* InteractionPrompt = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	FText Text_Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	FText Text_Target_Name;

private:

	// Returns target name
	UFUNCTION()
	FText GetTargetName();

	// Returns message text
	UFUNCTION()
	FText GetMessageText();

};
