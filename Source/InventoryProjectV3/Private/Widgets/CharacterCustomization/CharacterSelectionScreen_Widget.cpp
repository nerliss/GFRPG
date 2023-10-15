// Oleksandr Tkachov 2022-2023


#include "Widgets/CharacterCustomization/CharacterSelectionScreen_Widget.h"
#include "Widgets/CharacterCustomization/CharacterSelectionScreenOption.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Characters/PreviewCharacter.h"
#include "Materials/Material.h"
#include "GameFramework/InputSettings.h"

UCharacterSelectionScreen_Widget::UCharacterSelectionScreen_Widget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CharacterCustomizationData = nullptr;
	CharacterPreviewActor = nullptr;
	CharacterPreviewActorClassToSpawn = nullptr;
	CharacterSelectionScreenOptionClass = nullptr;
	CharacterPreviewMaterial = nullptr;

	bRightMouseButtonDown = false;
}

void UCharacterSelectionScreen_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UCharacterSelectionScreen_Widget::CloseWindow);
	}

	SpawnPreviewCharacter();

	CharactersList->ClearChildren();

	for (auto& SelectionData : CharacterCustomizationData->GetCharacterSelectionData())
	{
		auto* CharacterOption = Cast<UCharacterSelectionScreenOption>(CreateWidget(GetWorld(), CharacterSelectionScreenOptionClass));
		if (!CharacterOption)
		{
			UE_LOG(LogTemp, Error, TEXT("[UCharacterSelectionScreen_Widget::NativeConstruct] CharacterOption was not created!"));
			continue;
		}

		CharacterOption->AssociatedCharacterSelectionData = SelectionData;
		CharacterOption->OnButtonHoveredDelegate.AddDynamic(this, &UCharacterSelectionScreen_Widget::OnOptionButtonHovered);

		CharactersList->AddChild(CharacterOption);

		FSlateBrush Brush;
		Brush.SetResourceObject(CharacterPreviewMaterial);

		CharacterPreview->SetBrush(Brush);
	}
}

FReply UCharacterSelectionScreen_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bRightMouseButtonDown = true;
	}

	return FReply::Handled();
}

FReply UCharacterSelectionScreen_Widget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bRightMouseButtonDown = false;
	}

	return FReply::Handled();
}

void UCharacterSelectionScreen_Widget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bRightMouseButtonDown = false;
}

FReply UCharacterSelectionScreen_Widget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

FReply UCharacterSelectionScreen_Widget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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

void UCharacterSelectionScreen_Widget::SpawnPreviewCharacter()
{
	if (!CharacterPreviewActorClassToSpawn)
	{
		UE_LOG(LogTemp, Error, TEXT("[UCharacterSelectionScreen_Widget::SpawnPreviewCharacter] CharacterPreviewActorClassToSpawn is not set!"));
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

void UCharacterSelectionScreen_Widget::OnOptionButtonHovered(UCharacterSelectionScreenOption* Option)
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

void UCharacterSelectionScreen_Widget::CloseWindow()
{
	FInputModeGameOnly InputMode;

	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->bShowMouseCursor = false;

	RemoveFromParent();
	CharacterPreviewActor->Destroy();
}
