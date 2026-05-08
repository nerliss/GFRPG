// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPGTargetingPreviewActor.generated.h"

UCLASS()
class INVENTORYPROJECTV3_API ARPGTargetingPreviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ARPGTargetingPreviewActor();

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

protected:
	
	virtual void BeginPlay() override;

};
