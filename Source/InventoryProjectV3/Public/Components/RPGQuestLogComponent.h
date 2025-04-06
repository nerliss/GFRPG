// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGQuestLogComponent.generated.h"


class ARPGCharacter;
class ARPGQuest;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGQuestLogComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	URPGQuestLogComponent();

	TArray<ARPGQuest*> GetActiveQuests() const { return ActiveQuests; }

	ARPGQuest* GetCurrentActiveQuest() const { return CurrentActiveQuest; }

	void SetActiveQuest(ARPGQuest* Quest, bool bPlaySound);

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TArray<ARPGQuest*> ActiveQuests;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TArray<ARPGQuest*> CompletedQuests;

	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	ARPGQuest* CurrentActiveQuest;

	// QuestLogRef

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	ARPGCharacter* PlayerRef;

};
