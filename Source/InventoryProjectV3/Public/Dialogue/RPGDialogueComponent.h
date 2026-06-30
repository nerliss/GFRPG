// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dialogue/RPGDialogueData.h"
#include "RPGDialogueComponent.generated.h"

class ARPGPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueNodeChanged, const FDialogueNode&, Node);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	URPGDialogueComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	URPGDialogueData* DialogueData;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(ARPGPlayerCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SelectResponse(int32 ResponseIndex);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	TArray<FDialogueResponse> GetVisibleResponses() const;

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	bool IsDialogueActive() const { return bDialogueActive; }

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	FDialogueNode GetCurrentNode() const;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueNodeChanged OnNodeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnDialogueEnded OnDialogueEnded;

protected:

	virtual void BeginPlay() override;

private:

	void GoToNode(int32 NodeIndex);
	void HandleEffect(const FDialogueResponse& Response);
	int32 DetermineEntryNode() const;

	UPROPERTY(Transient)
	ARPGPlayerCharacter* PlayerRef;

	int32 CurrentNodeIndex = 0;
	bool bDialogueActive = false;
		
};
