// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/RPGInteract_Interface.h"
#include "RPGCharacter.generated.h"

class URPGAbilityComponent;
class URPGPointOfInterestComponent;
class URPGQuestLogComponent;
class URPGStatsComponent;
class UTexture2D;

UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
	Male,
	Female,
	Undefined
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterMoved);

UCLASS()
class INVENTORYPROJECTV3_API ARPGCharacter : public ACharacter, public IRPGInteract_Interface
{
	GENERATED_BODY()

public:

	ARPGCharacter();

	URPGQuestLogComponent* GetQuestLogComponent() const { return QuestLogComponent; }
	URPGStatsComponent* GetStatsComponent() const { return StatsComponent; }
	
	FText GetCharacterName() const { return CharacterName; }
	
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* IRPGInteract_Interface */
	virtual void InteractNative(AActor* Interactor) override;
	virtual FText GetNameNative() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	FText CharacterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	URPGQuestLogComponent* QuestLogComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	URPGStatsComponent* StatsComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	URPGPointOfInterestComponent* PointOfInterestComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	URPGAbilityComponent* AbilityComponent;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Movement")
	FOnCharacterMoved OnCharacterMoved;
};
