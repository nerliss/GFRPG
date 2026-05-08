// Oleksandr Tkachov 2021-2026


#include "Widgets/Quests/RPGQuestObjectiveItemWidget.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGInventory_Component.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/TextBlock.h"
#include "Items/RPGItem_Base.h"

void URPGQuestObjectiveItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// TODO: Replace with string table entity for future localization
	const FText StatusText = Objective.bCompleted ? FText::FromString("Completed: ") : FText::FromString("- ");
	ObjectiveStatusText->SetText(StatusText);
	ObjectiveStatusText->SetColorAndOpacity(Objective.bCompleted ? CompleteColor : ActiveColor);
	ObjectiveTrackerText->SetColorAndOpacity(Objective.bCompleted ? CompleteColor : ActiveColor);
	ObjectiveDescriptionText->SetColorAndOpacity(Objective.bCompleted ? CompleteColor : ActiveColor);
	ObjectiveDescriptionText->SetText(Objective.Description);

	UpdateTrackerData();
}

void URPGQuestObjectiveItemWidget::UpdateTrackerData()
{
	const ARPGPlayerCharacter* RPGCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!RPGCharacter)
	{
		return;
	}
	
	switch (Objective.Type)
	{
	case OT_Max:
	case OT_Location:
	case OT_Interact:
		{
			ObjectiveTrackerText->SetText(FText::GetEmpty());
		}
		break;
	case OT_Collect:
		{
			const ARPGItem_Base* ObjectiveItem = Cast<ARPGItem_Base>(Objective.Target.Get());
			int32 ObjectiveItemsInInventory, EmptyInt;
		
			if (RPGCharacter->GetInventoryComponent()->QueryInventory(ObjectiveItem->GetClass(), Objective.Amount, EmptyInt, ObjectiveItemsInInventory))
			{
				const int32 ClampedItemsInInventory = FMath::Clamp(ObjectiveItemsInInventory, 0, Objective.Amount);
				const FText FormattedText = FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(ClampedItemsInInventory), FText::AsNumber(Objective.Amount));
				ObjectiveTrackerText->SetText(FormattedText);
			}
		}
		break;
	case OT_Kill:
		{
			const int32 ObjectiveKillCount = RPGCharacter->GetQuestLogComponent()->GetCurrentActiveQuest()->GetKillCountCurrent();
			const FText FormattedText = FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(ObjectiveKillCount), FText::AsNumber(Objective.Amount));
			ObjectiveTrackerText->SetText(FormattedText);
		}
		break;
	default:
		break;
	}
}
