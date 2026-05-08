// Oleksandr Tkachov 2022-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/RPGReputation_Component.h"
#include "Engine/DataAsset.h"
#include "RPGDialogueData.generated.h"

class ARPGQuest;

UENUM(BlueprintType)
enum class EDialogueResponseEffect : uint8
{
    None,
    AcceptQuest,
    TurnInQuest,
    AddReputation,
    OpenTrade,
    EndConversation
};

USTRUCT(BlueprintType)
struct FDialogueResponse
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText ResponseText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    int32 NextNodeIndex = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    EDialogueResponseEffect Effect = EDialogueResponseEffect::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Reputation",
        meta = (EditCondition = "Effect == EDialogueResponseEffect::AddReputation"))
    TEnumAsByte<EFactions> ReputationFaction = Faction_Humans;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Reputation",
        meta = (EditCondition = "Effect == EDialogueResponseEffect::AddReputation"))
    float ReputationAmount = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Quest",
        meta = (EditCondition = "Effect == EDialogueResponseEffect::AcceptQuest || Effect == EDialogueResponseEffect::TurnInQuest"))
    TSubclassOf<ARPGQuest> QuestClass;

    // This response is hidden unless player has this quest active. nullptr = always visible
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Conditions")
    TSubclassOf<ARPGQuest> RequiredActiveQuest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Conditions")
    bool bHideAfterSelection = false;

    // Runtime flag 
    UPROPERTY(Transient)
    bool bWasSelected = false;
};

USTRUCT(BlueprintType)
struct FDialogueNode
{
    GENERATED_BODY()

    // Human-readable tag for your own reference in the editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FName NodeTag = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText SpeakerText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    USoundBase* VoiceLine = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    TArray<FDialogueResponse> Responses;
};

/**
 * 
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGDialogueData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// Node 0 is always the default entry point
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<FDialogueNode> Nodes;

	// Override which node to start at when this NPC has a completable quest
	// -1 means no override, fall back to node 0
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue|Entry Points")
	int32 QuestTurnInEntryNode = -1;

	// Override entry node for when a quest was already completed with this NPC
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue|Entry Points")
	int32 PostQuestEntryNode = -1;

	bool IsValidNodeIndex(int32 Index) const { return Nodes.IsValidIndex(Index); }
};
