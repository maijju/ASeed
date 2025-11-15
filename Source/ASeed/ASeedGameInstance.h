// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ASeedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	FName Difficulty = "Normal";

public:
	void SetDifficulty(FName Diff)
	{
		Difficulty = Diff;
	}
	FName GetDifficulty()
	{
		return Difficulty;
	}
};
