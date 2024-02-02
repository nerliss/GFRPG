// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Characters/RPGPlayerCharacter.h"
#include "RPGSaveGameObject.generated.h"

/**
 * Base class for save game object
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGSaveGameObject : public USaveGame
{
	GENERATED_BODY()
	
	URPGSaveGameObject();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player")
	FCharacterSelectionData CharacterPlayerData;

	/* MyTODO: Temporary a separate variable, combine with other similar variables in to a FStruct or something */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	FTransform PlayerTransform;

};
