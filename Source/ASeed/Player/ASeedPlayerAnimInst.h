// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ASeedPlayerAnimInst.generated.h"

/**
 * 
 */
UENUM()
enum class EMontageType
{
	Attack,
	Reload
};

UCLASS()
class ASEED_API UASeedPlayerAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
	TMap<EMontageType, TObjectPtr<UAnimMontage>> MontageMap;

public:
	void PlayMontageByType(EMontageType Type, float PlaySpeed = 1.0f)
	{
		if (IsAnyMontagePlaying())
			return;
		TObjectPtr<UAnimMontage>* Montage = MontageMap.Find(Type);
		Montage_Play(Montage->Get(), PlaySpeed);
	}

	UFUNCTION()
	void AnimNotify_AttackL();
	UFUNCTION()
	void AnimNotify_AttackR();
	UFUNCTION()
	void AnimNotify_Reloaded();
};
