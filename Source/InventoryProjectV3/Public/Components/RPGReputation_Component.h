// Oleksandr Tkachov 2022-2024

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
	Faction_Demons	UMETA(DisplayName = "Demons")
};

USTRUCT(BlueprintType)
struct FReputations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputations Struct")
	TEnumAsByte<EFactions> Faction_name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputations Struct")
	float Reputation_Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reputations Struct")
	float Reputation_Cap;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGReputation_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPGReputation_Component();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FReputations> Reputations;

	// Return reputation amount of desired faction
	UFUNCTION(BlueprintCallable, Category = "Reputation|Functions")
	void GetReputation(const TEnumAsByte<EFactions> In_Name /*input value*/,
		float& Out_Amount, int32& Out_Index, TEnumAsByte<EFactions>& Out_Name /*output values*/);

	UFUNCTION(BlueprintCallable, Category = "Reputation|Functions")
	void AddReputation(TEnumAsByte<EFactions> Name, float Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
