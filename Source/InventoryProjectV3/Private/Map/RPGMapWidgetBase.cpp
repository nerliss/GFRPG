// Oleksandr Tkachov 2022-2025


#include "Map/RPGMapWidgetBase.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Map/RPGMapIconComponent.h"
#include "Map/RPGMapSubsystem.h"
#include "Utility/LogDefinitions.h"

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

	// TODO: Remove when the map starts working
	if (MapSubsystem.IsValid() && MapImage && !MapImage->GetBrush().GetResourceObject())
	{
		UE_LOG(LogTemp, Display, TEXT("Map Brush refresh"));
		RefreshMapBrush();
	}
	
	if (!MapImage)
	{
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMapWidgetBase::NativeTick] No map image!"));
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
		UE_LOG(LogRPGMap, Error, TEXT("[URPGMapWidgetBase::RefreshMapBrush] MapSystem or MapImage is not valid"))
		return;
	}

	UTexture* MapTexture = MapSubsystem->MapTexture.LoadSynchronous();
	if (MapTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(MapTexture);
		Brush.ImageSize = FVector2D(MapTexture->GetSurfaceWidth(), MapTexture->GetSurfaceHeight());
		MapImage->SetBrush(Brush);
		UE_LOG(LogRPGMap, Log, TEXT("[URPGMapWidgetBase::RefreshMapBrush] Setting texture (%s) as a MAP brush"), *MapTexture->GetName());

		if (FogImage)
		{
			FSlateBrush FogBrush = Brush;
			FogImage->SetBrush(FogBrush);
			UE_LOG(LogRPGMap, Log, TEXT("[URPGMapWidgetBase::RefreshMapBrush] Setting texture (%s) as a FOG brush"), *FogImage->GetName());
		}
	}
}

void URPGMapWidgetBase::RefreshIcons()
{
	if (!MapSubsystem.IsValid() || !IconCanvas || !MapImage)
	{
		return;
	}

	// Rebuild icon list each tick - may be should be optimized later
	IconCanvas->ClearChildren();

	const FVector2D MapSize = MapImage->GetBrush().ImageSize * Zoom;
	
	for (const TWeakObjectPtr<URPGMapIconComponent>& WeakIcon : MapSubsystem->GetIcons())
	{
		URPGMapIconComponent* Icon = WeakIcon.Get();
		if (!Icon || !Icon->GetOwner())
		{
			continue;
		}

		FVector2D UV;
		if (!Icon->GetMapUV(UV))
		{
			continue;
		}

		UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		IconCanvas->AddChild(Image);

		if (Icon->Icon)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(Icon->Icon);
			Brush.ImageSize = Icon->IconSize;
			Brush.TintColor = Icon->Tint;
			Image->SetBrush(Brush);
		}

		// Convert UV to pixel and center the icon
		const FVector2D Pixel = (UV * MapSize) - (Icon->IconSize / 2.f);

		if (UCanvasPanelSlot* ImageSlot = Cast<UCanvasPanelSlot>(Image->Slot))
		{
			ImageSlot->SetAutoSize(false);
			ImageSlot->SetPosition(Pixel);
			ImageSlot->SetSize(Icon->IconSize);
			ImageSlot->SetAlignment(FVector2D(0.f, 0.f));
		}

		if (Icon->bRotateWithActor)
		{
			Image->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
			Image->SetRenderTransformAngle(Icon->GetOwner()->GetActorRotation().Yaw);
		}
	}
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
