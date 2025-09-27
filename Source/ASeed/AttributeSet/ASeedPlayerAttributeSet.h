// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASeedAttributeSet.h"
#include "ASeedPlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedPlayerAttributeSet : public UASeedAttributeSet
{
	GENERATED_BODY()

public:
	virtual void CallbackAmmo();
	virtual void CallbackHP(bool IsDead);
	virtual void CallbackAttack();
	virtual void CallbackDefense();
	
};
