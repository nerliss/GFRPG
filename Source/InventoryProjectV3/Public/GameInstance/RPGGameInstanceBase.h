// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstanceBase.generated.h"

class URPGSaveGameObject;

/**
 * GFRPG main game instance
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

	URPGGameInstanceBase();
	
	virtual void Init() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FString SaveSlotName;

	UPROPERTY(BlueprintReadWrite, Category = "SaveData")
	URPGSaveGameObject* SaveGameObject;

	void InitializeSaveGameObject();

public:

	FString GetSaveSlotName() const;

	URPGSaveGameObject* GetSaveGameObject() const;

	UFUNCTION(BlueprintCallable)
	void SavePlayer();

	UFUNCTION(BlueprintCallable)
	void LoadPlayer();

};
