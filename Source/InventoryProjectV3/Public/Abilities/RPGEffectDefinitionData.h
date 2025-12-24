// Oleksandr Tkachov 2022-2026

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPGEffectDefinitionData.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGEffectDefinitionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FText Id;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FText Name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability System")
	TSoftObjectPtr<UTexture2D> Icon;
};
