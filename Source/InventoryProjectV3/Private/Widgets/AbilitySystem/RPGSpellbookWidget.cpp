// Oleksandr Tkachov 2021-2026


#include "Widgets/AbilitySystem/RPGSpellbookWidget.h"

#include "Abilities/RPGAbilityDefinitionData.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetRegistryModule.h"

void URPGSpellbookWidget::NativeConstruct()
{
	// Load before Super since we currently work with Blueprint logic
	// TODO: Move after Super when all the logic is in C++
	LoadAllAbilitiesFromRegistry();
	
	Super::NativeConstruct();	
}

void URPGSpellbookWidget::LoadAllAbilitiesFromRegistry()
{
	IAssetRegistry& AssetRegistry =	FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	
	FARFilter Filter;
	Filter.PackagePaths.Add(TEXT("/Game/InventoryProject/Core/AbilitySystem/Data/Abilities/"));
	Filter.ClassPaths.Add(URPGAbilityDefinitionData::StaticClass()->GetClassPathName());
	Filter.bRecursivePaths = true;
	Filter.bRecursiveClasses = true;

	TArray<FAssetData> FoundAssets;
	AssetRegistry.GetAssets(Filter, FoundAssets);

	UE_LOG(LogTemp, Warning, TEXT("Found %d assets at path"), FoundAssets.Num());
	
	AllAbilitiesInGame.Empty();

	for (const FAssetData& AssetData : FoundAssets)
	{
		URPGAbilityDefinitionData* Definition = Cast<URPGAbilityDefinitionData>(AssetData.GetAsset());

		if (Definition)
		{
			AllAbilitiesInGame.Add(Definition);
		}
	}
}

void URPGSpellbookWidget::PopulatePage()
{
}

void URPGSpellbookWidget::UpdateButtons()
{
}
