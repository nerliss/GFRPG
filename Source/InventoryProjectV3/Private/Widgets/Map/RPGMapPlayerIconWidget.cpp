// Oleksandr Tkachov 2022-2024


#include "Widgets/Map/RPGMapPlayerIconWidget.h"

void URPGMapPlayerIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateIconRotation();
}

void URPGMapPlayerIconWidget::UpdateIconRotation()
{
	const auto* PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn)
	{
		return;
	}

	const float PlayerRotation = PlayerPawn->GetActorRotation().Yaw;
	SetRenderTransformAngle(PlayerRotation);
}
