// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_Stat.h"

void UASeedUI_Stat::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AttackText = Cast<UTextBlock>(GetWidgetFromName(FName("AttackText")));
	DefenseText = Cast<UTextBlock>(GetWidgetFromName(FName("DefenseText")));
	CooldownReduceText = Cast<UTextBlock>(GetWidgetFromName(FName("CooldownReduceText")));
	AttackSpeedText = Cast<UTextBlock>(GetWidgetFromName(FName("AttackSpeedText")));
	MoveSpeedText = Cast<UTextBlock>(GetWidgetFromName(FName("MoveSpeedText")));
	ExpBonusText = Cast<UTextBlock>(GetWidgetFromName(FName("ExpBonusText")));
	CoreBonusText = Cast<UTextBlock>(GetWidgetFromName(FName("CoreBonusText")));
}