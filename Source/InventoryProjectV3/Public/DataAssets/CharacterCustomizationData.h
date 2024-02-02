// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Characters/RPGPlayerCharacter.h"
#include "CharacterCustomizationData.generated.h"

/**
 *	Data asset that hold character customization data (e.g. Character model, anim blueprint, portrait, name etc.)
 */
UCLASS()
class INVENTORYPROJECTV3_API UCharacterCustomizationData : public UDataAsset
{
	GENERATED_BODY()
	
	UCharacterCustomizationData();

protected:

	/* Characters list */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Selection")
	TArray<FCharacterSelectionData> CharacterSelectionData;

public:

	TArray<FCharacterSelectionData> GetCharacterSelectionData() const;

};
