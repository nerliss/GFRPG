// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/RPGInteract_Interface.h"
#include "RPGCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
	Male,
	Female,
	Undefined
};

UCLASS()
class INVENTORYPROJECTV3_API ARPGCharacter : public ACharacter, public IRPGInteract_Interface
{
	GENERATED_BODY()

public:

	ARPGCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* IRPGInteract_Interface */
	virtual void InteractNative(AActor* Interactor) override;
	virtual FText GetNameNative() const override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Character")
	FText CharacterName;

};
