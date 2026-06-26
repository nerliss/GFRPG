// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/RPGInteractInterface.h"
#include "RPGMountBase.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class INVENTORYPROJECTV3_API ARPGMountBase : public ACharacter, public IRPGInteractInterface
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
	void OnRightMoved(const float Value);
	void TurnAtRate(const float Rate);
	void LookUpAtRate(const float Rate);
	void OnDismount();
	void OnMount(AActor* Interactor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mount")
	FName AttachmentSocket;

private:

	bool bMounted;

};
