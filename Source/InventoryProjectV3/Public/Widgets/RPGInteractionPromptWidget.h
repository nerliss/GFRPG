// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGInteractionPromptWidget.generated.h"

class USizeBox;
class UVerticalBox;
class UBorder;
class UTextBlock;

UCLASS()
class INVENTORYPROJECTV3_API URPGInteractionPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
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

	// Is used instead of NativePreConstruct()
	bool Initialize() override;

	UFUNCTION()
	FText GetTargetName();

	UFUNCTION()
	FText GetMessageText();

};
