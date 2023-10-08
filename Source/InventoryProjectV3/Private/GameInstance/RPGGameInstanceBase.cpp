// Destruction Games. 2022


#include "GameInstance/RPGGameInstanceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Save/RPGSaveGameObject.h"

URPGGameInstanceBase::URPGGameInstanceBase()
{
	// Save data 
	SaveSlotName.Empty();
	SaveGameObject = nullptr;
}

void URPGGameInstanceBase::Init()
{
	Super::Init();

	InitializeSaveGameObject();
}

void URPGGameInstanceBase::InitializeSaveGameObject()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveGameObject = Cast<URPGSaveGameObject>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		SaveGameObject = Cast<URPGSaveGameObject>(UGameplayStatics::CreateSaveGameObject(URPGSaveGameObject::StaticClass()));
	}
}

FString URPGGameInstanceBase::GetSaveSlotName() const
{
	return SaveSlotName;
}

URPGSaveGameObject* URPGGameInstanceBase::GetSaveGameObject() const
{
	return SaveGameObject;
}
