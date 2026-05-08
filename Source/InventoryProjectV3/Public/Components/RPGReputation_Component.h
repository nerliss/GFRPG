// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGReputation_Component.generated.h"

UENUM(BlueprintType)
enum EFactions
{
	Faction_Orcs	UMETA(DisplayName = "Orcs"),
	Faction_Humans	UMETA(DisplayName = "Humans"),
	Faction_Elves	UMETA(DisplayName = "Elves"),
	Faction_Dwarfs	UMETA(DisplayName = "Dwarfs"),
	Faction_Demons	UMETA(DisplayName = "Demons"),
	Faction_Max		UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FReputations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputations")
	TEnumAsByte<EFactions> FactionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputations")
	float CurrentReputation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputations")
	float MaxReputation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGReputation_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URPGReputation_Component();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FReputations> Reputations;
	
	UFUNCTION(BlueprintCallable, Category = "Reputation")
	void GetReputation(const TEnumAsByte<EFactions> FactionToGet, float& OutAmount, int32& OutIndex, TEnumAsByte<EFactions>& OutName);

	UFUNCTION(BlueprintCallable, Category = "Reputation")
	void AddReputation(TEnumAsByte<EFactions> Name, float Amount);

protected:

	virtual void BeginPlay() override;
};
