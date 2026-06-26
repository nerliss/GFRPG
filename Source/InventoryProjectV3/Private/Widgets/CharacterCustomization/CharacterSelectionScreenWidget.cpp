// Oleksandr Tkachov 2021-2026


#include "Widgets/CharacterCustomization/CharacterSelectionScreenWidget.h"
#include "Widgets/CharacterCustomization/CharacterSelectionScreenOptionWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Characters/PreviewCharacter.h"
#include "Materials/Material.h"
#include "GameFramework/InputSettings.h"

UCharacterSelectionScreenWidget::UCharacterSelectionScreenWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CharacterCustomizationData = nullptr;
	CharacterPreviewActor = nullptr;
	CharacterPreviewActorClassToSpawn = nullptr;
	CharacterSelectionScreenOptionClass = nullptr;
	CharacterPreviewMaterial = nullptr;

	bRightMouseButtonDown = false;
}

void UCharacterSelectionScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UCharacterSelectionScreenWidget::CloseWindow);
	}

	SpawnPreviewCharacter();

	CharactersList->ClearChildren();

	for (auto& SelectionData : CharacterCustomizationData->GetCharacterSelectionData())
	{
		auto* CharacterOption = Cast<UCharacterSelectionScreenOptionWidget>(CreateWidget(GetWorld(), CharacterSelectionScreenOptionClass));
		if (!CharacterOption)
		{
			UE_LOG(LogTemp, Error, TEXT("[UCharacterSelectionScreenWidget::NativeConstruct] CharacterOption was not created!"));
			continue;
		}

		CharacterOption->AssociatedCharacterSelectionData = SelectionData;
		CharacterOption->OnButtonHoveredDelegate.AddDynamic(this, &UCharacterSelectionScreenWidget::OnOptionButtonHovered);

		CharactersList->AddChild(CharacterOption);

		FSlateBrush Brush;
		Brush.SetResourceObject(CharacterPreviewMaterial);

		CharacterPreview->SetBrush(Brush);
	}
}

FReply UCharacterSelectionScreenWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bRightMouseButtonDown = true;
	}

	return FReply::Handled();
}

FReply UCharacterSelectionScreenWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bRightMouseButtonDown = false;
	}

	return FReply::Handled();
}

void UCharacterSelectionScreenWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bRightMouseButtonDown = false;
}

FReply UCharacterSelectionScreenWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (bRightMouseButtonDown)
	{
		if (CharacterPreviewActor)
		{
			const float InvertedMouseX = InMouseEvent.GetCursorDelta().X * -1.f;
			const FRotator RotationDelta = FRotator(0.f, InvertedMouseX, 0.f);
			
			// Add rotation to preview model
			CharacterPreviewActor->AddPreviewModelRotation(RotationDelta);
		}
	}

	return FReply::Handled();
}

FReply UCharacterSelectionScreenWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	TArray<FInputActionKeyMapping> KeyMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName("TogglePauseMenu", KeyMappings);

	for (auto& Key : KeyMappings)
	{
		if (Key.Key == InKeyEvent.GetKey())
		{
			CloseWindow();
			break;
		}
	}

	return FReply::Handled();
}

void UCharacterSelectionScreenWidget::SpawnPreviewCharacter()
{
	if (!CharacterPreviewActorClassToSpawn)
	{
		UE_LOG(LogTemp, Error, TEXT("[UCharacterSelectionScreenWidget::SpawnPreviewCharacter] CharacterPreviewActorClassToSpawn is not set!"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0.f, 0.f, 1000000.f));

	CharacterPreviewActor = GetWorld()->SpawnActor<APreviewCharacter>(CharacterPreviewActorClassToSpawn, SpawnTransform, SpawnParams);

	CharacterPreviewActor->AssociatedCharacterCustomizationData = CharacterCustomizationData;
	CharacterPreviewActor->OnAssociatedCharacterDataUpdatedDelegate.Broadcast();
}

void UCharacterSelectionScreenWidget::OnOptionButtonHovered(UCharacterSelectionScreenOptionWidget* Option)
{
	auto OptionSelectionData = Option->AssociatedCharacterSelectionData;

	CharacterPreviewActor->GetPreviewMesh()->SetSkeletalMeshAsset(OptionSelectionData.SkeletalMesh);
	CharacterPreviewActor->GetPreviewMesh()->SetAnimInstanceClass(OptionSelectionData.AssociatedAnimBP);
	CharacterPreviewActor->UpdateCameraDistance(OptionSelectionData.PreviewCameraDistanceDelta);
	CharacterPreviewActor->LoadTextures();

	FSlateBrush Brush;
	Brush.SetResourceObject(CharacterPreviewMaterial);

	CharacterPreview->SetBrush(Brush);
}

void UCharacterSelectionScreenWidget::CloseWindow()
{
	FInputModeGameOnly InputMode;

	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->bShowMouseCursor = false;

	RemoveFromParent();
	CharacterPreviewActor->Destroy();
}
