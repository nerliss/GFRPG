// Destruction Games. 2022


#include "Characters/PreviewCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"

APreviewCharacter::APreviewCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PreviewMesh->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	DefaultCameraDistance = FVector(-190.f, 0.f, 75.f);

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SceneCaptureComponent->SetRelativeLocation(DefaultCameraDistance);

	AssociatedCharacterCustomizationData = false;
}

void APreviewCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnAssociatedCharacterDataUpdatedDelegate.AddDynamic(this, &APreviewCharacter::OnAssociatedCharacterDataUpdated);
}

void APreviewCharacter::ResetCameraDistance()
{
	const auto CurrentCharacterData = FindCurrentCharacterData();

	SceneCaptureComponent->AddRelativeLocation(CurrentCharacterData.PreviewCameraDistanceDelta);
}

void APreviewCharacter::UpdateCameraDistance(FVector DistanceDelta)
{
	SceneCaptureComponent->SetRelativeLocation(DefaultCameraDistance);
	SceneCaptureComponent->AddLocalOffset(DistanceDelta);
}

FCharacterSelectionData APreviewCharacter::FindCurrentCharacterData() const
{
	if (!AssociatedCharacterCustomizationData)
	{
		UE_LOG(LogTemp, Error, TEXT("[APreviewCharacter::FindCurrentCharacterData] AssociatedCharacterCustomizationData is null, please initialize it before calling this function!"));
		return FCharacterSelectionData();
	}

	const auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter)
	{ 
		return FCharacterSelectionData();
	}

	const TArray<FCharacterSelectionData> LocalSelectionData = AssociatedCharacterCustomizationData->GetCharacterSelectionData();

	for (auto& SelectionData : LocalSelectionData)
	{
		if (SelectionData.SkeletalMesh == PlayerCharacter->GetMesh()->GetSkeletalMeshAsset())
		{
			return SelectionData;
		}
	}

	return FCharacterSelectionData();
}

void APreviewCharacter::AddPreviewModelRotation(FRotator RotationDelta)
{
	PreviewMesh->AddLocalRotation(RotationDelta);
}

void APreviewCharacter::LoadTextures()
{
	TArray<UMaterialInterface*> MeshMaterials = PreviewMesh->GetMaterials();

	for (const auto& Material : MeshMaterials)
	{
		if (!Material)
		{
			continue;
		}

		Material->SetForceMipLevelsToBeResident(true, true, -1.f);
	}
}

void APreviewCharacter::OnAssociatedCharacterDataUpdated()
{
	const auto CurrentCharacterData = FindCurrentCharacterData();
	PreviewMesh->SetSkeletalMesh(CurrentCharacterData.SkeletalMesh);
	PreviewMesh->SetAnimInstanceClass(CurrentCharacterData.AssociatedAnimBP);
	
	ResetCameraDistance();

	LoadTextures();
}

