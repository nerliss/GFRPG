// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGSpellbookWidget.generated.h"

class URPGAbilityDefinitionData;
/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGSpellbookWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;
	
	void LoadAllAbilitiesFromRegistry();
	
	UPROPERTY(BlueprintReadOnly)
	TArray<URPGAbilityDefinitionData*> AllAbilitiesInGame;
	
};
