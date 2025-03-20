// Oleksandr Tkachov 2022-2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGGenericStatics.generated.h"

/**
 *
 */
UCLASS()
class INVENTORYPROJECTV3_API URPGGenericStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	URPGGenericStatics();

	UFUNCTION(BlueprintCallable)
	static UTexture2D* LoadTextureFromFile(const FString& ImagePath);

	static void ApplyTextureToMaterial(UTexture2D* NewTexture, UMaterialInstanceDynamic* DynamicMat);

	UFUNCTION(BlueprintCallable)
	static FString OpenFileDialog();

	UFUNCTION(BlueprintCallable)
	static void ApplyMaterialToMesh(UStaticMeshComponent* MeshComponent, UMaterialInterface* BaseMaterial);

};
