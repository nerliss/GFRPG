// Oleksandr Tkachov 2022-2025


#include "DataAssets/CharacterCustomizationData.h"

UCharacterCustomizationData::UCharacterCustomizationData()
{
	CharacterSelectionData.Empty();
}

TArray<FCharacterSelectionData> UCharacterCustomizationData::GetCharacterSelectionData() const
{
	return CharacterSelectionData;
}
