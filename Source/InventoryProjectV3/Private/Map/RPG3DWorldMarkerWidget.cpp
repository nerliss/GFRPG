// Oleksandr Tkachov 2022-2025


#include "Map/RPG3DWorldMarkerWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void URPG3DWorldMarkerWidget::UpdateDistance(FText DistanceAsText, bool bMapMarker, float PlayerDistance,
                                             float MaxDistance)
{
	if (DistanceText)
	{
		DistanceText->SetText(DistanceAsText);
	}

	if (bMapMarker)
	{
		// Some logic for compass
	}
	else
	{
		if (MarkerBorder)
		{
			FLinearColor Color = FLinearColor::White;
			Color.A = 1.f - ((PlayerDistance - (MaxDistance * 0.5f)) / MaxDistance * 0.5f); // TODO: Break down in to meaningful parts
			MarkerBorder->SetContentColorAndOpacity(Color);
		}
	}
}

void URPG3DWorldMarkerWidget::UpdateName(FText Name)
{
	if (MarkerText)
	{
		MarkerText->SetText(Name);
	}
}
