// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ASeedUI_Stat.generated.h"

/**
 *
 */

UCLASS()
class ASEED_API UASeedUI_Stat : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();

public:
	TObjectPtr<UTextBlock> AttackText;
	TObjectPtr<UTextBlock> DefenseText;
	TObjectPtr<UTextBlock> CooldownReduceText;
	TObjectPtr<UTextBlock> AttackSpeedText;
	TObjectPtr<UTextBlock> MoveSpeedText;
	TObjectPtr<UTextBlock> ExpBonusText;
	TObjectPtr<UTextBlock> CoreBonusText;
};
