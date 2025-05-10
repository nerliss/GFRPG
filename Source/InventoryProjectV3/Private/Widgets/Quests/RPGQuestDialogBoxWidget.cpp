// Oleksandr Tkachov 2022-2025


#include "Widgets/Quests/RPGQuestDialogBoxWidget.h"

#include "Characters/RPGPlayerCharacter.h"
#include "Components/RPGInventory_Component.h"
#include "Components/RPGQuestLogComponent.h"
#include "Components/RPGXP_Component.h"
#include "Items/RPGItem_Base.h"
#include "PlayerController/RPGPlayer_Controller.h"
#include "Quests/RPGQuest.h"
#include "Widgets/RPGHUD_Widget.h"

void URPGQuestDialogBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// TODO: Move from BP
}

bool URPGQuestDialogBoxWidget::CheckPlayerInventory() const
{
	if (!Quest)
	{
		return false;
	}
	
	const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return false;
	}

	const ARPGPlayer_Controller* RPGPC = Cast<ARPGPlayer_Controller>(GetOwningPlayer());
	if (!RPGPC)
	{
		return false;
	}
	
	const URPGHUD_Widget* HUDWidget = Cast<URPGHUD_Widget>(RPGPC->GetHUDWidget());
	if (!HUDWidget)
	{
		return false;
	}
	
	PlayerCharacter->GetQuestLogComponent()->CheckPlayerInventory(Quest);
	// TODO: URPGHUD_Widget::UpdateObjectivesList

	// TODO: Figure out how to update a button visibility providing that the button is only available to one of BPs, so we can't exactly bind it 
	return PlayerCharacter->GetQuestLogComponent()->MarkQuestComplete(Quest->GetClass());
}

void URPGQuestDialogBoxWidget::RemoveQuestItemsFromInventory()
{
	if (!Quest)
	{
		return;
	}

	const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return;
	}
	
	for (FObjectiveData Objective : Quest->GetObjectives())
	{
		if (Objective.Type == OT_Collect)
		{
			const ARPGItem_Base* Item = Cast<ARPGItem_Base>(Objective.Target.Get());
			if (Item)
			{
				PlayerCharacter->GetInventoryComponent()->RemoveItem(Item->GetClass(), Objective.Amount);
			}
		}
	}
}

void URPGQuestDialogBoxWidget::OnTurnedInClicked()
{
	if (!Quest)
	{
		return;
	}

	const ARPGPlayer_Controller* RPGPC = Cast<ARPGPlayer_Controller>(GetOwningPlayer());
	if (!RPGPC)
	{
		return;
	}
	
	const URPGHUD_Widget* HUDWidget = Cast<URPGHUD_Widget>(RPGPC->GetHUDWidget());
	if (!HUDWidget)
	{
		return;
	}
	
	const ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	URPGQuestLogComponent* QuestLog = PlayerCharacter->GetQuestLogComponent();
	if (!QuestLog)
	{
		return;
	}
	
	Quest->OnQuestCompeted();
	
	PlayerCharacter->XPComp->AddXP(Quest->GetXPReward());
	// TODO: Add money reward
	QuestLog->DeleteQuest(Quest);

	// TODO: Post sound
	
	RemoveQuestItemsFromInventory();

	if (QuestLog->GetCurrentActiveQuest() == Quest || QuestLog->GetActiveQuests().IsEmpty())
	{
		// TODO: Set QuestName text to empty in HUDWidget->QuestHUDObjectives->QuestName
	}

	RemoveWidget();
}

void URPGQuestDialogBoxWidget::OnAcceptClicked()
{
	if (!Quest)
	{
		return;
	}
	
	ARPGPlayerCharacter* PlayerCharacter = Cast<ARPGPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	if (Quest->GetObjectives()[0].Type == OT_Collect)
	{
		PlayerCharacter->GetQuestLogComponent()->CheckPlayerInventory(Quest);
	}

	auto CompletedQuests = PlayerCharacter->GetQuestLogComponent()->GetCompleteQuests();
	auto ActiveQuests = PlayerCharacter->GetQuestLogComponent()->GetActiveQuests();
	if (!CompletedQuests.Find(Quest) && !ActiveQuests.Find(Quest))
	{
		PlayerCharacter->GetQuestLogComponent()->AddQuest(Quest);
		PlayerCharacter->GetQuestLogComponent()->SetActiveQuest(Quest);
		// TODO: Play accept quest sound
		RemoveWidget();
	}
}

void URPGQuestDialogBoxWidget::RemoveWidget()
{
	ARPGPlayer_Controller* RPGPC = Cast<ARPGPlayer_Controller>(GetOwningPlayer());
	if (!RPGPC)
	{
		return;
	}
	RPGPC->SetDefaultInputMode();
	
	RemoveFromParent();
}
