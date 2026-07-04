// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RPGInteractInterface.generated.h"

UINTERFACE(Blueprintable)
class URPGInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class INVENTORYPROJECTV3_API IRPGInteractInterface
{
	GENERATED_BODY()

public:

	/* Interact with an actor DEPRECATED. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* Interactor);

	/* Get name of an interactable actor DEPRECATED. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	FText GetName();

	/* Interact with an actor. C++ only version. */
	virtual void InteractNative(AActor* Interactor) = 0;

	/* Get name of an interactable actor. C++ only version. */
	virtual FText GetNameNative() const { return FText::GetEmpty(); }

};
