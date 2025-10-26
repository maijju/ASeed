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
	Reload,
	Rolling,
	Die
};

UCLASS()
class ASEED_API UASeedPlayerAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	TObjectPtr<class AASeedPlayer> Owner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montages")
	TMap<EMontageType, TObjectPtr<UAnimMontage>> MontageMap;

	UPROPERTY(BlueprintReadOnly)
	FName RollingSectionName;

protected:
	virtual void NativeInitializeAnimation() override;

public:
	void PlayMontageByType(EMontageType Type, float PlaySpeed = 1.0f)
	{
		TObjectPtr<UAnimMontage>* Montage = MontageMap.Find(Type);
		if (Montage)
		{
			// Rolling can cancel auto
			if (IsAnyMontagePlaying() && Type != EMontageType::Rolling)
				return;

			Montage_Play(Montage->Get(), PlaySpeed);
			if (Type == EMontageType::Rolling)
			{
				Montage_JumpToSection(RollingSectionName, Montage->Get());
			}
		}
	}

	void SetRollingSectionName(FName SectionName)
	{
		RollingSectionName = SectionName;
	}

	UFUNCTION()
	void AnimNotify_AttackL();
	UFUNCTION()
	void AnimNotify_AttackR();
	UFUNCTION()
	void AnimNotify_Reloaded();
	UFUNCTION()
	void AnimNotify_GameOver();
};
