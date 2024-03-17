// Oleksandr Tkachov 2022-2024

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

/* Player camera point of view */
UENUM(BlueprintType)
enum class EPlayerPOV : uint8
{
	FirstPerson		UMETA(DisplayName = "First Person"),
	ThirdPerson		UMETA(DisplayName = "Third Person")
};

UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
	Male,
	Female,
	Undefined
};

/* Character selection data */
USTRUCT(BlueprintType)
struct FCharacterSelectionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Selection")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Selection")
	ECharacterGender Gender;

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
		Gender = ECharacterGender::Undefined;
		SkeletalMesh = nullptr;
		AssociatedAnimBP = nullptr;
		Portrait = nullptr;
		PreviewCameraDistanceDelta = FVector::Zero();
	}

	friend FArchive& operator<<(FArchive& Ar, FCharacterSelectionData& CharacterSelectionData);
};

// MyTODO: Move all FCharacterSelectionData related info to a separate header
inline FArchive& operator<<(FArchive& Ar, FCharacterSelectionData& CharacterSelectionData)
{
	// For storing asset paths during serialization
	FString SkeletalMeshPath;
	FString AnimBPPath;
	ECharacterGender CharacterGender;

	if (Ar.IsSaving())
	{
		// Convert the raw pointers to asset paths for saving
		SkeletalMeshPath = CharacterSelectionData.SkeletalMesh ? CharacterSelectionData.SkeletalMesh->GetPathName() : "";
		AnimBPPath = CharacterSelectionData.AssociatedAnimBP ? CharacterSelectionData.AssociatedAnimBP->GetPathName() : "";
		CharacterGender = CharacterSelectionData.Gender;
	}

	// Serialize the asset paths instead of raw pointers
	Ar << SkeletalMeshPath;
	Ar << AnimBPPath;
	Ar << CharacterGender;

	if (Ar.IsLoading())
	{
		// Convert the asset paths back to raw pointers when loading
		CharacterSelectionData.SkeletalMesh = SkeletalMeshPath.IsEmpty() ? nullptr : LoadObject<USkeletalMesh>(nullptr, *SkeletalMeshPath);
		CharacterSelectionData.AssociatedAnimBP = AnimBPPath.IsEmpty() ? nullptr : LoadClass<UAnimInstance>(nullptr, *AnimBPPath);
		CharacterSelectionData.Gender = CharacterGender;
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
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Death();

	void SetPOV(const EPlayerPOV DesiredPOV);

#if WITH_EDITORONLY_DATA
	static FString GetLastSavedCharacterFileName() { return LastSavedCharacterFileName; }
#endif

	float GetStealthedMaxWalkSpeed() const { return StealthedMaxWalkSpeed; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void BlueprintOnStealthPressed();

	UFUNCTION(BlueprintGetter, Category = "Character")
	ECharacterGender GetCharacterGender() const { return CharacterGender; }

	void SetCharacterGender(const ECharacterGender NewGender) { CharacterGender = NewGender; }

	void SetMainHUDWidget(URPGHUD_Widget* NewWidget) { MainHUD_WidgetRef = NewWidget; }
	URPGHUD_Widget* GetMainHUDWidget() const { return MainHUD_WidgetRef; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
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

	/* Is currently in stealth mode? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bStealthed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mount")
	bool bMounted;

protected:

	void OnForwardMoved(float Value);
	void OnRightMoved(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void OnJumpStarted();
	void OnJumpEnded();

	void OnSprintStarted();
	void OnSprintStopped();

	void OnStealthPressed();

	/* Calculates and applies fall damage. 
	 * Tweak FallDamageMultiplier and FallDamageMinimalThreshold to customize damage amount. 
	 */
	void CalculateFallDamage();

	/* Called in blueprints to add some cosmetic stuff that is difficult to implement in C++ to death event
	 * (i.e. timelines, widgets that are created in BP etc.)
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnDied();

	/** Switch camera POV to Third Person if in First Person and vise versa */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void OnPOVSwitched();

	UFUNCTION(BlueprintImplementableEvent, Category = "Camera")
	void OnPOVSwitchedBlueprint();
	
	AActor* TraceForInteractableObjects(const float InTraceLength);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnInventoryToggled();

	void LoadLastCharacterModel();

	// MyTODO: Move to RPGGameStatics or something
	UFUNCTION(BlueprintGetter, Category = "Misc")
	URPGGameInstanceBase* GetRPGGameInstance() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	EPlayerPOV PlayerPOV;

	/* Character sounds collection */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	UCharacterSoundCollection* CharacterSoundCollection;

	/* Can this character be damaged by falling? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	bool bCanGetDamagedFromFalling;

	/* The value Z velocity's percent is multiplied by. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta=(ClampMin="0.1", ClampMax="6.0"))
	float FallDamageMultiplier;

	/* The minimal threshold to actually get damage from falling. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float FallDamageMinimalThreshold;

	// MyTODO: Move this to RPGAnimInstance
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AActor* InteractActor;

	/* Linetrace length that changes depending on POV */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float TraceLength;

	UPROPERTY(BlueprintReadWrite, Category = "Dialog")
	bool bInDialog;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float DefaultMaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintMaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float StealthedMaxWalkSpeed;

	/* Character gender, updated when new character model is set. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character")
	ECharacterGender CharacterGender;

private:

	/* Attempt to interact with the current InteractActor */
	void OnInteractPressed();

	/* Reference to HUD Widget. Set in RPGPlayer_Controller. */
	UPROPERTY()
	URPGHUD_Widget* MainHUD_WidgetRef;

#if WITH_EDITORONLY_DATA
	/* Editor-only function that loads last saved character into character blueprint itself to be displayed in the editor itself, not only in the game */
	void LoadLastCharacterModelInternal();
#endif

	/* Camera boom length limits. X = Min, Y = Max*/
	FVector2D TargetBoomLengthLimits;

#if WITH_EDITORONLY_DATA
	/* Name for an external file to write/read character data from (for editor only) */
	inline static FString LastSavedCharacterFileName = TEXT("TestCharacterSelectionData.bin");
#endif

};
