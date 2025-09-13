// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPGMapIconComponent.generated.h"

class UTexture2D;
class URPGMapSubsystem;

UENUM(BlueprintType)
enum class EMapIconCategory : uint8
{
	Player,
	Quest,
	Shop,
	Enemy,
	Chest,
	Waypoint,
	Custom
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECTV3_API URPGMapIconComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URPGMapIconComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Returns UV (0..1) for this icon’s world position. */
	bool GetMapUV(FVector2D& OutUV) const;
	
	/** Icon to draw on the map/minimap. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	UTexture2D* Icon = nullptr;

	/** Size in pixels in the map widget. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	FVector2D IconSize = FVector2D(24.f, 24.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	FLinearColor Tint = FLinearColor::White;

	/** If true, rotate icon by owner’s yaw (useful for player arrow). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	bool bRotateWithActor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	EMapIconCategory Category = EMapIconCategory::Custom;

private:
	
	TWeakObjectPtr<URPGMapSubsystem> MapSubsystem;
};
