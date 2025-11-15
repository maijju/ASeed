// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ASeedAbility.h"
#include "ASeedGA_PlayerRampage.generated.h"

/**
 *
 */
UCLASS()
class ASEED_API UASeedGA_PlayerRampage : public UASeedAbility
{
	GENERATED_BODY()

public:
	UASeedGA_PlayerRampage();

protected:
	/*---------INIT---------*/
	class AASeedPlayer* Player;
	class UASeedPlayerAnimInst* AnimInst;
	UAbilitySystemComponent* PlayerASC;
	const class UASeedAttributeSet* PlayerAttr;

	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo;
	FGameplayAbilityActivationInfo CachedActivationInfo;

	bool bRampaging = false;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	UFUNCTION()
	void RepeatFire(UAnimMontage* Montage, bool bInterrupted);

};
