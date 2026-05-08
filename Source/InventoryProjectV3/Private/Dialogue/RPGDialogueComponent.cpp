// Oleksandr Tkachov 2021-2026


#include "Dialogue/RPGDialogueComponent.h"

URPGDialogueComponent::URPGDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DialogueData = nullptr;
	PlayerRef = nullptr;
	CurrentNodeIndex = 0;
	bDialogueActive = false;
}

void URPGDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URPGDialogueComponent::StartDialogue(ARPGPlayerCharacter* Player)
{
}

void URPGDialogueComponent::SelectResponse(int32 ResponseIndex)
{
}

void URPGDialogueComponent::EndDialogue()
{
}

TArray<FDialogueResponse> URPGDialogueComponent::GetVisibleResponses() const
{
	return TArray<FDialogueResponse>();
}

FDialogueNode URPGDialogueComponent::GetCurrentNode() const
{
	if (DialogueData && DialogueData->IsValidNodeIndex(CurrentNodeIndex))
	{
		return DialogueData->Nodes[CurrentNodeIndex];
	}
	
	return FDialogueNode();
}

void URPGDialogueComponent::GoToNode(int32 NodeIndex)
{
}

void URPGDialogueComponent::HandleEffect(const FDialogueResponse& Response)
{
}

int32 URPGDialogueComponent::DetermineEntryNode() const
{
	return 0;
}

