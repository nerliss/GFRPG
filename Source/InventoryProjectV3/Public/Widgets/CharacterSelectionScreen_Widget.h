// Destruction Games. 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionScreen_Widget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API UCharacterSelectionScreen_Widget : public UUserWidget
{
	GENERATED_BODY()
	
	UCharacterSelectionScreen_Widget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};