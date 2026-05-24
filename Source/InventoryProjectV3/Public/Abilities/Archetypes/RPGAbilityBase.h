// Oleksandr Tkachov 2021-2026

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RPGAbilityComponent.h"
#include "RPGAbilityBase.generated.h"

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
	virtual bool UseAbility(FRPGTargetData& TargetData); 
	virtual URPGAbilityDefinitionData* GetAbilityDefinition() const { return AbilityDefinition; }
	
	// Channeling
	virtual void OnChannelStart(FRPGTargetData& TargetData);
	virtual void OnChannelTick(FRPGTargetData& TargetData);
	virtual void OnChannelEnd(FRPGTargetData& TargetData, EAbilityInterruptReason Reason);
	
	// Casting
	virtual void OnCastStart(FRPGTargetData& TargetData);
	virtual void OnCastComplete(FRPGTargetData& TargetData);
	virtual void OnCastInterrupted(FRPGTargetData& TargetData, EAbilityInterruptReason Reason);
	
	// Toggling
	virtual void OnToggleStarted(FRPGTargetData& TargetData);
	virtual void OnToggleEnded(FRPGTargetData& TargetData);
	
	// Preview Targeting (used when EAbilityTargetingFlow::PreviewConfirm)
	virtual void OnTargetingPreviewStarted(FRPGTargetData& TargetData);
	virtual void OnTargetingPreviewUpdated(FRPGTargetData& TargetData);
	virtual void OnTargetingPreviewConfirmed(FRPGTargetData& TargetData);
	virtual void OnTargetingPreviewCanceled(FRPGTargetData& TargetData);
	
	// TODO: Implement EAbilityTargetingFlow::HoldRelease
	
	// TODO: Remove ReadWrite access once moving to C++ is done
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	URPGAbilityDefinitionData* AbilityDefinition;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	URPGAbilityComponent* AbilityComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	AActor* OwnerActor;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	float CooldownEndTime;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bIsChanneling;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	float ChannelEndTime;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	float NextTickTime;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	float ChannelTickPeriod;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bRequiresHold;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bUpdateTargetEachTick;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	FRPGTargetData ActiveAbilityTargetData;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bTickOnStart;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bInterruptOnMove;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bIsCasting;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	float CastStartTime;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	float CastEndTime;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bLockTargetAtCastStart;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bPayCostOnStart;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bStartCooldownOnStart;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bIsToggled;
	
};
