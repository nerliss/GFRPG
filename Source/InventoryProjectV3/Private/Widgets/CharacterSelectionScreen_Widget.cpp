// Destruction Games. 2022


#include "Widgets/CharacterSelectionScreen_Widget.h"

UCharacterSelectionScreen_Widget::UCharacterSelectionScreen_Widget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CharacterCustomizationData = nullptr;
}

bool UCharacterSelectionScreen_Widget::Initialize()
{
	return Super::Initialize();
}

void UCharacterSelectionScreen_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCharacterSelectionScreen_Widget::NativeDestruct()
{
	Super::NativeDestruct();
}
