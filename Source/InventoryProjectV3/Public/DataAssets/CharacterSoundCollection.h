// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterSoundCollection.generated.h"

class UAkAudioEvent;

/* Data asset that stores diffent character sounds */
UCLASS()
class INVENTORYPROJECTV3_API UCharacterSoundCollection : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	/* Sound that will be played when character dies */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pain Sounds")
	UAkAudioEvent* DeathSound;

};
