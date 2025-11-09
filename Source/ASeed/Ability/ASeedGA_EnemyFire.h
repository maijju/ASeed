// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASeedAbility.h"
#include "ASeedGA_EnemyFire.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedGA_EnemyFire : public UASeedAbility
{
	GENERATED_BODY()

public:
	UASeedGA_EnemyFire();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	
};
