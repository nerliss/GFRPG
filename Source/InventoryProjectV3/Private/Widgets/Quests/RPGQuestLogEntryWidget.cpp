// Oleksandr Tkachov 2022-2025


#include "Widgets/Quests/RPGQuestLogEntryWidget.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/RPGQuestLogComponent.h"
#include "Slate/SlateBrushAsset.h"

void URPGQuestLogEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EntryButton)
	{
		EntryButton->OnClicked.AddDynamic(this, &URPGQuestLogEntryWidget::OnEntryButtonClicked);
	}

	// TODO: Test this since GetBorderStatus originally was bound to EntryBorder's Brush
	if (EntryBorder)
	{
		EntryBorder->SetBrush(GetBorderStatus());
	}
}

void URPGQuestLogEntryWidget::OnEntryButtonClicked()
{
	const ARPGPlayerCharacter* RPGPlayer = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!RPGPlayer)
	{
		return;
	}

	if (RPGPlayer->GetQuestLogComponent()->GetCurrentActiveQuest() == Quest)
	{
		RPGPlayer->GetQuestLogComponent()->SetActiveQuest(Quest, false);
		
		// TODO: Post AK Button Click sound
	}
}

FSlateBrush URPGQuestLogEntryWidget::GetBorderStatus() const
{
	const ARPGPlayerCharacter* RPGPlayer = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!RPGPlayer)
	{
		return FSlateBrush();
	}

	if (RPGPlayer->GetQuestLogComponent()->GetCurrentActiveQuest() == Quest)
	{
		return ActiveSlateBrush ? ActiveSlateBrush->Brush : FSlateBrush();
	}

	return InactiveSlateBrush ? InactiveSlateBrush->Brush : FSlateBrush();
}
