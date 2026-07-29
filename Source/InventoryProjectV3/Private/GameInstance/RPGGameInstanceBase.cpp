// Oleksandr Tkachov 2021-2026


#include "GameInstance/RPGGameInstanceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Save/RPGSaveGameObject.h"
#include "Utility/LogDefinitions.h"
#include "Utility/Utility.h"

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
	
	LOG_WITH_FUNCTION_NAME(LogRPGSaving, Verbose, TEXT("SaveGameObject initialized"));
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

	const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		return;
	}

	SaveGameObject->PlayerTransform = PlayerCharacter->GetActorTransform();

	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);
	LOG_WITH_FUNCTION_NAME(LogRPGSaving, Log, TEXT("Saving player to slot %s"), *SaveSlotName);
}

void URPGGameInstanceBase::LoadPlayer()
{
	if (!SaveGameObject)
	{
		return;
	}

	ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		return;
	}

	// TODO: Figure this out
	PlayerCharacter->SetActorTransform(SaveGameObject->PlayerTransform);
	UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0); 
	LOG_WITH_FUNCTION_NAME(LogRPGSaving, Log, TEXT("Loading player to slot %s"), *SaveSlotName);
}
