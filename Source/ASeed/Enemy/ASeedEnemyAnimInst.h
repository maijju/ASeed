// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ASeedEnemyAnimInst.generated.h"

UENUM(BlueprintType)
enum class EEnemyAnimState : uint8
{
	Idle,
	Walk,
	Run,
	Attack,
	Hit,
	Stun,
	Die
};

UCLASS()
class ASEED_API UASeedEnemyAnimInst : public UAnimInstance
{
	GENERATED_BODY()

protected:
	TObjectPtr<class AASeedEnemy> Owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, TObjectPtr<UAnimSequence>> SeqMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyAnimState AnimState;

protected:
	virtual void NativeInitializeAnimation() override;

public:
	void InitSeqMap(TMap<FName, TObjectPtr<UAnimSequence>> Map)
	{
		SeqMap = Map;
	}

	void SetAnimState(EEnemyAnimState State)
	{
		AnimState = State;
	}

public:
	UFUNCTION()
	void AnimNotify_AttackImpact();

	UFUNCTION()
	void AnimNotify_AttackEnd();
};
