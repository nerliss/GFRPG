// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapWidgetBase.h"

void URPGMapWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void URPGMapWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void URPGMapWidgetBase::RefreshMapBrush()
{
	
}

void URPGMapWidgetBase::RefreshIcons()
{
	
}

bool URPGMapWidgetBase::ComputeMapTranslation(FVector2D& OutTranslation, float& OutRotationDeg) const
{
	return false;
}
