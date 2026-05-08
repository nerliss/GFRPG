// Oleksandr Tkachov 2021-2026


#include "DataAssets/CharacterCustomizationData.h"

UCharacterCustomizationData::UCharacterCustomizationData()
{
	CharacterSelectionData.Empty();
}

TArray<FCharacterSelectionData> UCharacterCustomizationData::GetCharacterSelectionData() const
{
	return CharacterSelectionData;
}
