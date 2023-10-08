// Destruction Games. 2022


#include "Widgets/CharacterCustomization/CharacterSelectionScreenOption.h"
#include "Components/Button.h"
#include "Characters/RPGPlayerCharacter.h"

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

	// MyTODO: Move from blueprints saving selected character feature 
}

FText UCharacterSelectionScreenOption::GetButtonText() const
{
	return AssociatedCharacterSelectionData.Name;
}
