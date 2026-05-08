// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGMainGameMode.generated.h"

class UTexture2D;
class UTextureRenderTarget2D;

UCLASS()
class INVENTORYPROJECTV3_API ARPGMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ARPGMainGameMode();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSoftObjectPtr<UTexture2D> MapTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	UTextureRenderTarget2D* FogRT;
};
