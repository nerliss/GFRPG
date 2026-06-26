// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/RPGInteractInterface.h"
#include "RPGCharacter.generated.h"

class URPGEffectsComponent;
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
class INVENTORYPROJECTV3_API ARPGCharacter : public ACharacter, public IRPGInteractInterface
{
	GENERATED_BODY()

public:

	ARPGCharacter();

	URPGQuestLogComponent* GetQuestLogComponent() const { return QuestLogComponent; }
	URPGStatsComponent* GetStatsComponent() const { return StatsComponent; }
	URPGAbilityComponent* GetAbilityComponent() const { return AbilityComponent; }
	URPGEffectsComponent* GetEffectComponent() const { return EffectsComponent; }
	URPGPointOfInterestComponent* GetPointOfInterestComponent() const { return PointOfInterestComponent; }
	
	FText GetCharacterName() const { return CharacterName; }
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Movement")
	FOnCharacterMoved OnCharacterMoved;
	
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* IRPGInteractInterface */
	virtual void InteractNative(AActor* Interactor) override;
	virtual FText GetNameNative() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	FText CharacterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	URPGQuestLogComponent* QuestLogComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	URPGStatsComponent* StatsComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	URPGPointOfInterestComponent* PointOfInterestComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	URPGAbilityComponent* AbilityComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	URPGEffectsComponent* EffectsComponent;
	
};
