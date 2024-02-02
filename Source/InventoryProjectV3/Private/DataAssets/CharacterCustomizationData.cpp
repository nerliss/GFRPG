// Oleksandr Tkachov 2022-2024


#include "DataAssets/CharacterCustomizationData.h"

UCharacterCustomizationData::UCharacterCustomizationData()
{
	// Set default values
	CharacterSelectionData.Empty();
}

TArray<FCharacterSelectionData> UCharacterCustomizationData::GetCharacterSelectionData() const
{
	return CharacterSelectionData;
}
