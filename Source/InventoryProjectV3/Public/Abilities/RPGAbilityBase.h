// Oleksandr Tkachov 2022-2026

#pragma once

#include "CoreMinimal.h"
#include "RPGAbilityBase.generated.h"

class URPGAbilityDefinitionData;

UCLASS(BlueprintType, Blueprintable)
class INVENTORYPROJECTV3_API URPGAbilityBase : public UObject
{
	GENERATED_BODY()
	
public:	
	
	URPGAbilityBase();

	//virtual void Init();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	URPGAbilityDefinitionData* AbilityDefinition;
	
};
