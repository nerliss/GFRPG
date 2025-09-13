// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapWidgetBase.h"

#include "Components/Image.h"
#include "Map/RPGMapSubsystem.h"

void URPGMapWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetWorld() && GetWorld()->GetGameInstance())
	{
		MapSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<URPGMapSubsystem>();
	}

	RefreshMapBrush();
}

void URPGMapWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!MapImage)
	{
		return;
	}

	FVector2D Translation;
	float RotationDeg = 0.f;

	if (ComputeMapTranslation(Translation, RotationDeg))
	{
		MapImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));;
		MapImage->SetRenderScale(FVector2D(Zoom, Zoom));
		MapImage->SetRenderTransformAngle(bRotateWithPlayer ? RotationDeg : 0.f);
		MapImage->SetRenderTranslation(Translation);

		if (FogImage)
		{
			FogImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));;
			FogImage->SetRenderScale(FVector2D(Zoom, Zoom));
			FogImage->SetRenderTransformAngle(bRotateWithPlayer ? RotationDeg : 0.f);
			FogImage->SetRenderTranslation(Translation);
		}
	}

	RefreshIcons();
}

void URPGMapWidgetBase::RefreshMapBrush()
{
	if (!MapSubsystem.IsValid() || !MapImage)
	{
		return;
	}

	UTexture* MapTexture = MapSubsystem->MapTexture.LoadSynchronous();
	if (MapTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(MapTexture);
		Brush.ImageSize = FVector2D(MapTexture->GetSurfaceWidth(), MapTexture->GetSurfaceHeight());
		MapImage->SetBrush(Brush);

		if (FogImage)
		{
			FSlateBrush FogBrush = Brush;
			FogImage->SetBrush(FogBrush);
		}
	}
}

void URPGMapWidgetBase::RefreshIcons()
{
	
}

bool URPGMapWidgetBase::ComputeMapTranslation(FVector2D& OutTranslation, float& OutRotationDeg) const
{
	OutRotationDeg = 0.0f;

	if (!bRotateWithPlayer || !MapImage)
	{
		OutTranslation = FVector2D::ZeroVector;
		return true;
	}

	// Player-centered rotation\translation (like minimap)
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			FVector2D PlayerUV;
			if (MapSubsystem.IsValid() && MapSubsystem->WorldToMapUV(Pawn->GetActorLocation(), PlayerUV))
			{
				const FVector2D MapSize = MapImage->GetBrush().ImageSize * Zoom;
				const FVector2D WidgetSize = GetDesiredSize();
				const FVector2D ViewHalf = WidgetSize / 2;
				const FVector2D PlayerPx = PlayerUV + MapSize;

				OutTranslation = ViewHalf + PlayerPx;
				OutRotationDeg = Pawn->GetActorRotation().Yaw;
				return true;
			}
		}
	}
	
	return false;
}
