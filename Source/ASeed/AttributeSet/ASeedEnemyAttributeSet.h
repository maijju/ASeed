// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASeedAttributeSet.h"
#include "ASeedEnemyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedEnemyAttributeSet : public UASeedAttributeSet
{
	GENERATED_BODY()

public:
	virtual void CallbackHP(bool IsDead);
	
};
