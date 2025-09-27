// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASeedAbility.h"
#include "ASeedGA_PlayerFire.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedGA_PlayerFire : public UASeedAbility
{
	GENERATED_BODY()

public:
	UASeedGA_PlayerFire();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
