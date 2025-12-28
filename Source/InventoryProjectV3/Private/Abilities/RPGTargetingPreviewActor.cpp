// Oleksandr Tkachov 2022-2026


#include "Abilities/RPGTargetingPreviewActor.h"

ARPGTargetingPreviewActor::ARPGTargetingPreviewActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(SceneRoot);
}

void ARPGTargetingPreviewActor::BeginPlay()
{
	Super::BeginPlay();
}

