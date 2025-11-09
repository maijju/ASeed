// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ASeedUI_Status.generated.h"

/**
 * 
 */

UCLASS()
class ASEED_API UASeedUI_Status : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();

public:
	TObjectPtr<UTextBlock> AttackText;
	TObjectPtr<UTextBlock> DefenseText;
	TObjectPtr<UTextBlock> MaxHPText;
	TObjectPtr<UTextBlock> MaxAmmoText;
	TObjectPtr<UTextBlock> CooldownReduceText;
	TObjectPtr<UTextBlock> AttackSpeedText;
	TObjectPtr<UTextBlock> MoveSpeedText;
	TObjectPtr<UTextBlock> ExpBonusText;
	TObjectPtr<UTextBlock> CoreBonusText;
};
