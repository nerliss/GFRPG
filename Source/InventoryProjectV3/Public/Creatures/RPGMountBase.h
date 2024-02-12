// Oleksandr Tkachov 2022-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/RPGInteract_Interface.h"
#include "RPGMountBase.generated.h"

UCLASS()
class INVENTORYPROJECTV3_API ARPGMountBase : public ACharacter, public IRPGInteract_Interface
{
	GENERATED_BODY()

public:

	ARPGMountBase();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void InteractNative(AActor* Interactor) override;
	virtual FText GetNameNative() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mount")
	FText Name {FText::FromString(TEXT("Mount"))};

	void OnForwardMoved(const float Value);

private:

	bool bMounted {false};

};
