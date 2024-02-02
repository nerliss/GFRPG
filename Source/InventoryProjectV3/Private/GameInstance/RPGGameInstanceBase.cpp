// Oleksandr Tkachov 2022-2024


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
	SaveGameObject = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0) ? 
	Cast<URPGSaveGameObject>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)) : 
	Cast<URPGSaveGameObject>(UGameplayStatics::CreateSaveGameObject(URPGSaveGameObject::StaticClass()));
}

FString URPGGameInstanceBase::GetSaveSlotName() const
{
	return SaveSlotName;
}

URPGSaveGameObject* URPGGameInstanceBase::GetSaveGameObject() const
{
	return SaveGameObject;
}

void URPGGameInstanceBase::SavePlayer()
{
	if (!SaveGameObject)
	{
		return;
	}

	const auto* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		return;
	}

	SaveGameObject->PlayerTransform = PlayerCharacter->GetActorTransform();

	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);
}

void URPGGameInstanceBase::LoadPlayer()
{
	if (!SaveGameObject)
	{
		return;
	}

	auto* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		return;
	}

	PlayerCharacter->SetActorTransform(SaveGameObject->PlayerTransform);
}
