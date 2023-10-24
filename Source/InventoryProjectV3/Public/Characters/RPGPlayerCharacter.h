// Oleksandr Tkachov 2022-2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGPlayerCharacter.generated.h"

class URPGXP_Component;
class URPGHealth_Component;
class URPGReputation_Component;
class URPGInventory_Component;
class UCameraComponent;
class USpringArmComponent;
class UAkComponent;
class AActor;
class UAkAudioEvent;
class UAnimMontage;
class URPGHUD_Widget;
class UCharacterSoundCollection;
class UAnimInstance;
class USkeletalMesh;
class URPGGameInstanceBase;

DECLARE_LOG_CATEGORY_EXTERN(LogRPGPlayerCharacter, Log, All);

/* Player camera point of view */
UENUM(BlueprintType)
enum class EPlayerPOV : uint8
{
	FirstPerson		UMETA(DisplayName = "First Person"),
	ThirdPerson		UMETA(DisplayName = "Third Person")
};

/* Character selection data */
USTRUCT(BlueprintType)
struct FCharacterSelectionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Selection")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Selection")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Selection")
	TSubclassOf<UAnimInstance> AssociatedAnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Selection")
	UTexture2D* Portrait;

	/* How much should we offset preview camera by? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Selection")
	FVector PreviewCameraDistanceDelta;

	FCharacterSelectionData()
	{
		Name = FText::FromString("Default");
		SkeletalMesh = nullptr;
		AssociatedAnimBP = nullptr;
		Portrait = nullptr;
		PreviewCameraDistanceDelta = FVector::ZeroVector;
	}

	friend FArchive& operator<<(FArchive& Ar, FCharacterSelectionData& CharacterSelectionData);
};

// MyTODO: Move all FCharacterSelectionData related info to a separate header
inline FArchive& operator<<(FArchive& Ar, FCharacterSelectionData& CharacterSelectionData)
{
	// For storing asset paths during serialization
	FString SkeletalMeshPath;
	FString AnimBPPath;

	if (Ar.IsSaving())
	{
		// Convert the raw pointers to asset paths for saving
		SkeletalMeshPath = CharacterSelectionData.SkeletalMesh ? CharacterSelectionData.SkeletalMesh->GetPathName() : "";
		AnimBPPath = CharacterSelectionData.AssociatedAnimBP ? CharacterSelectionData.AssociatedAnimBP->GetPathName() : "";
	}

	// Serialize the asset paths instead of raw pointers
	Ar << SkeletalMeshPath;
	Ar << AnimBPPath;

	if (Ar.IsLoading())
	{
		// Convert the asset paths back to raw pointers when loading
		CharacterSelectionData.SkeletalMesh = SkeletalMeshPath.IsEmpty() ? nullptr : LoadObject<USkeletalMesh>(nullptr, *SkeletalMeshPath);
		CharacterSelectionData.AssociatedAnimBP = AnimBPPath.IsEmpty() ? nullptr : LoadClass<UAnimInstance>(nullptr, *AnimBPPath);

		UE_LOG(LogTemp, Warning, TEXT("Loading SkeletalMesh from path: %s\n ABP from: %s"), *SkeletalMeshPath, *AnimBPPath);
	}

	return Ar;
}

UCLASS()
class INVENTORYPROJECTV3_API ARPGPlayerCharacter : public ACharacter
{
	
	GENERATED_BODY()

	friend class ARPGHealth_Component;

public:

	ARPGPlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;

#if WITH_EDITORONLY_DATA
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	/* Experience component stores everything about Experience, Skill Points, Skills and Talents (in future) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URPGXP_Component* XPComp;

	/* Health component responds for actors hit points, damage behavior etc. 
	 * MyTODO: Turn this component into Stat component that will be storing different player stats such as HP, MP, Strength, Agility etc.  
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URPGHealth_Component* HPComp;

	/* Reputation component stores all the data about factions and their possible rewards */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URPGReputation_Component* ReputationComp;

	/* Inventory component stores data about items */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URPGInventory_Component* InventoryComp;

	/* Ak audio component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAkComponent* AkComp;

	/* Character sounds collection */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	UCharacterSoundCollection* CharacterSoundCollection;

	/* Reference to HUD Widget - is set in RPGPlayer_Controller */
	UPROPERTY()
	URPGHUD_Widget* MainHUD_WidgetRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

protected:

	void OnForwardMoved(float Value);
	void OnRightMoved(float Value);

	void OnJumpStarted();
	void OnJumpEnded();

	void OnSprintStarted();
	void OnSprintStopped();

	/*
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/*
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

public:

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Death();

protected:

	/* Calculates and applies fall damage. 
	 * Tweak FallDamageMultiplier and FallDamageMinimalThreshold to customize damage amount. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void CalculateFallDamage();

	/* Can this character be damaged by falling? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bCanGetDamagedFromFalling;

	/* The value Z velocity's percent is multiplied by. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta=(ClampMin="0.1", ClampMax="6.0"))
	float FallDamageMultiplier;

	/* The minimal threshold to actually get damage from falling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float FallDamageMinimalThreshold;

	/* Called in blueprints to add some cosmetic stuff that is difficult to implement in C++ to death event
	 * (i.e. timelines, widgets that are created in BP etc.)
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnDied();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	UAnimMontage* DeathMontage;

	/** Switch camera POV to Third Person if in First Person and vise versa */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void OnPOVSwitched();

	UFUNCTION(BlueprintImplementableEvent, Category = "Camera")
	void OnPOVChangedBlueprint();

private:

	/* Camera boom lengths */
	float MaxTargetBoomLength;
	float MinTargetBoomLength;

protected:

	/* POV state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	EPlayerPOV PlayerPOV;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* TraceForInteractableObjects(float inTraceLength, bool bDrawDebugLine);

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AActor* InteractActor;

	/* Linetrace length that changes depending on POV */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction")
	float TraceLength;

private:

	/* Attempt to interact with the current InteractActor */
	void OnInteractPressed();

protected:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnInventoryToggled();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Dialog")
	bool bInDialog;

	void LoadLastCharacterModel();

#if WITH_EDITORONLY_DATA
private:
	/* Editor only selection data to be loaded from */
	UPROPERTY(Transient)
	FCharacterSelectionData LastCharacterSelectionDataInternal;

	/* Editor-only function that loads last saved character into character blueprint itself to be displayed in the editor itself, not only in the game */
	void LoadLastCharacterModelInternal();

public:
	void SetLastCharacterSelectionDataInternal(FCharacterSelectionData NewData);
#endif

protected:

	UFUNCTION(BlueprintCallable)
	URPGGameInstanceBase* GetRPGGameInstance() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float DefaultMaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintMaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float StealthedMaxWalkSpeed;

public:

	/* Is currently in stealth mode? */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bStealthed;

	void OnStealthPressed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void BlueprintOnStealthPressed();

};
