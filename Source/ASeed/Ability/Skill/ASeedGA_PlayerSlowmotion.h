// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ASeedAbility.h"
#include "ASeedGA_PlayerSlowmotion.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedGA_PlayerSlowmotion : public UASeedAbility
{
	GENERATED_BODY()
	
public:
	UASeedGA_PlayerSlowmotion();

protected:
	float Duration = 5.0f;
	float SlowFactor = 0.1f;

	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo;
	FGameplayAbilityActivationInfo CachedActivationInfo;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void EndDuration();
};
