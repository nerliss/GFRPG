// Destruction Games. 2022


#include "Widgets/CharacterSelectionScreen_Widget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Characters/PreviewCharacter.h"

UCharacterSelectionScreen_Widget::UCharacterSelectionScreen_Widget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CharacterCustomizationData = nullptr;
	CharacterPreviewActor = nullptr;
	CharacterPreviewActorClassToSpawn = nullptr;

	bRightMouseButtonDown = false;
}

bool UCharacterSelectionScreen_Widget::Initialize()
{
	return Super::Initialize();
}

void UCharacterSelectionScreen_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	CharactersList->ClearChildren();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0.f, 0.f, 1000000.f));

	CharacterPreviewActor = GetWorld()->SpawnActor<APreviewCharacter>(CharacterPreviewActorClassToSpawn, SpawnTransform, SpawnParams);

	CharacterPreviewActor->AssociatedCharacterCustomizationData = CharacterCustomizationData;
	CharacterPreviewActor->OnAssociatedCharacterDataUpdatedDelegate.Broadcast();
}

void UCharacterSelectionScreen_Widget::NativeDestruct()
{
	Super::NativeDestruct();
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
