// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "RPGAbilityBase.generated.h"

struct FRPGTargetData;
class URPGAbilityDefinitionData;
class URPGAbilityComponent;

UCLASS(BlueprintType, Blueprintable)
class INVENTORYPROJECTV3_API URPGAbilityBase : public UObject
{
	GENERATED_BODY()
	
public:	
	
	URPGAbilityBase();

	virtual void InitAbility(URPGAbilityComponent* FromAbilityComponent, AActor* FromOwnerActor, URPGAbilityDefinitionData* FromAbilityDefinitionData);
	virtual bool CanUseAbility();
	virtual bool TryUseAbility(FRPGTargetData TargetData); 
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	URPGAbilityDefinitionData* AbilityDefinition;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	URPGAbilityComponent* AbilityComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	AActor* OwnerActor;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	float CooldownEndTime;
	
};
