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

	// TODO: THIS CLASS IS UNUSED. DELETE LATER
	
	/** Icon to draw on the map/minimap */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface"), Category="MapIcon")
	//UTexture2D* Icon;
	TObjectPtr<UObject> Icon;

	/** Size in pixels in the map widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	FVector2D IconSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	FLinearColor Tint;

	/** If true, rotate icon by owner’s yaw (useful for player arrow) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	bool bRotateWithActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapIcon")
	EMapIconCategory Category;

private:
	
	TWeakObjectPtr<URPGMapSubsystem> MapSubsystem;
};
