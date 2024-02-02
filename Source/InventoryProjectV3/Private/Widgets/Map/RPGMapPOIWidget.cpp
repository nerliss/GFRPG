// Oleksandr Tkachov 2022-2024


#include "Widgets/Map/RPGMapPOIWidget.h"
#include "Components/RPGPointOfInterestComponent.h"
#include "Components/Image.h"

void URPGMapPOIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetPOIIcon();
}

void URPGMapPOIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

float URPGMapPOIWidget::FindAngleDegrees(FVector2D A, FVector2D B) const
{
	const float Angle = FMath::Atan2(A.Y - B.Y, A.X - B.X) * 180 / PI;
	return Angle;
}

FVector2D URPGMapPOIWidget::FindCoordinates(float Radius, float Degrees) const
{
	// MyTODO: Check this out
	const float X = Radius * FMath::Cos(Degrees * PI / 180);
	const float Y = Radius * FMath::Sin(Degrees * PI / 180);
	return FVector2D(X, Y);
}

void URPGMapPOIWidget::SetPOIIcon()
{
	if (!Owner)
	{
		return;
	}

	auto* OwnerPOIComponent = Owner->FindComponentByClass<URPGPointOfInterestComponent>();
	if (!OwnerPOIComponent)
	{
		return;
	}

	POIImage->SetBrush(OwnerPOIComponent->GetIcon());
}
