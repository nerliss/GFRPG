// Destruction Games. 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAssets/CharacterCustomizationData.h"
#include "PreviewCharacter.generated.h"

class UCharacterCustomizationData;
class USkeletalMeshComponent;
class USceneCaptureComponent2D;
class USceneComponent;

UCLASS()
class INVENTORYPROJECTV3_API APreviewCharacter : public AActor
{
	GENERATED_BODY()
	
public:	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAssociatedCharacterDataUpdatedDelegate);

	APreviewCharacter();

protected:

	virtual void PostInitializeComponents() override;

	/* Represents root component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RootComponent")
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* PreviewMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CaptureComponent")
	USceneCaptureComponent2D* SceneCaptureComponent;

public:	

	USkeletalMeshComponent* GetPreviewMesh() const { return PreviewMesh; }

	/* Character customizations list currently associated with this class */
	UPROPERTY(BlueprintReadWrite, Category = "Character Customization")
	UCharacterCustomizationData* AssociatedCharacterCustomizationData;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	FVector DefaultCameraDistance;

	/* Resets camera distance to CameraDistanceValue associated with current character */
	UFUNCTION(BlueprintCallable)
	void ResetCameraDistance();

public:

	/* Update camera distance to preview model by DistanceDelta associated with that Character Data */
	UFUNCTION(BlueprintCallable)
	void UpdateCameraDistance(FVector DistanceDelta);

protected:

	/* Return character selection data associated with current player model. 
	* Needs to be called after AssociatedCharacterCustomizationData is initialized.
	* Currently AssociatedCharacterCustomizationData is initialized upon spawning this actor in CharacterSelectionScreen_Widget.
	*/
	FCharacterSelectionData FindCurrentCharacterData() const;

public:

	/* Rotates character model. Currently called in CharacterSelectionScreen_Widget on mouse move */
	UFUNCTION(BlueprintCallable)
	void AddPreviewModelRotation(FRotator RotationDelta);

	/* Forces all skeletal mesh's materials and their textures to use highest mip level */
	UFUNCTION(BlueprintCallable)
	void LoadTextures();

	/* Called upon spawning and assigning AssociatedCharacterCustomizationData to this actor */
	UPROPERTY(BlueprintCallable, Category = "Character Customization")
	FOnAssociatedCharacterDataUpdatedDelegate OnAssociatedCharacterDataUpdatedDelegate;

	/* Bound to OnAssociatedCharacterDataUpdatedDelegate. This is a safe call on this actor spawn (instead of BeginPlay), 
	* so any BeginPlay related logic can and should be used here. 
	*/
	UFUNCTION()
	void OnAssociatedCharacterDataUpdated();
};
