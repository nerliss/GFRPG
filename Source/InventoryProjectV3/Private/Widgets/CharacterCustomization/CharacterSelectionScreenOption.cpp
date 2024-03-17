// Oleksandr Tkachov 2022-2024


#include "Widgets/CharacterCustomization/CharacterSelectionScreenOption.h"
#include "Components/Button.h"
#include "Characters/RPGPlayerCharacter.h"
#include "GameInstance/RPGGameInstanceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Save/RPGSaveGameObject.h"

void UCharacterSelectionScreenOption::NativeConstruct()
{
	Super::NativeConstruct();

	if (OptionButton)
	{
		OptionButton->OnHovered.AddDynamic(this, &UCharacterSelectionScreenOption::OnOptionButtonHovered);
		OptionButton->OnClicked.AddDynamic(this, &UCharacterSelectionScreenOption::OnOptionsButtonClicked);
	}
}

void UCharacterSelectionScreenOption::OnOptionButtonHovered()
{
	OnButtonHoveredDelegate.Broadcast(this);
}

void UCharacterSelectionScreenOption::OnOptionsButtonClicked()
{
	auto* PlayerCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayer()->GetPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	PlayerCharacter->GetMesh()->SetSkeletalMeshAsset(AssociatedCharacterSelectionData.SkeletalMesh);
	PlayerCharacter->GetMesh()->SetAnimInstanceClass(AssociatedCharacterSelectionData.AssociatedAnimBP);

	PlayerCharacter->SetCharacterGender(AssociatedCharacterSelectionData.Gender);

	auto* RPGGameInstance = Cast<URPGGameInstanceBase>(GetWorld()->GetGameInstance());
	if (!RPGGameInstance)
	{	
		UE_LOG(LogTemp, Error, TEXT("Game instance is invalid"));
		return;
	}

	RPGGameInstance->GetSaveGameObject()->CharacterPlayerData = AssociatedCharacterSelectionData;

#if WITH_EDITORONLY_DATA
	// Testing serialization
	FCharacterSelectionData TestCharacterSelectionData;
	TestCharacterSelectionData.AssociatedAnimBP = AssociatedCharacterSelectionData.AssociatedAnimBP;
	TestCharacterSelectionData.SkeletalMesh = AssociatedCharacterSelectionData.SkeletalMesh;

	TArray<uint8> Bytes;
	FMemoryWriter Writer(Bytes);
	Writer << TestCharacterSelectionData;

	FString FilePath = FPaths::ProjectSavedDir();
	FilePath += PlayerCharacter->GetLastSavedCharacterFileName();
	FFileHelper::SaveArrayToFile(Bytes, *FilePath);
#endif

	const bool bSaveSuccessful = UGameplayStatics::SaveGameToSlot(RPGGameInstance->GetSaveGameObject(), RPGGameInstance->GetSaveSlotName(), 0);

	FString SaveDebugMessage = bSaveSuccessful ? TEXT("Character saved") : TEXT("An error occured when trying to save selected character");
	UE_LOG(LogTemp, Warning, TEXT("%s"), *SaveDebugMessage);
}

FText UCharacterSelectionScreenOption::GetButtonText() const
{
	return AssociatedCharacterSelectionData.Name;
}
