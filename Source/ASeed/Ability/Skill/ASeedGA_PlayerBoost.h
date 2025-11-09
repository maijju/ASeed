// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ASeedAbility.h"
#include "ASeedGA_PlayerBoost.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedGA_PlayerBoost : public UASeedAbility
{
	GENERATED_BODY()

public:
	UASeedGA_PlayerBoost();

protected:
	float Duration = 5.f;
	float SpeedBonus = 1.3f;

	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo;
	FGameplayAbilityActivationInfo CachedActivationInfo;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void EndDuration();
};
