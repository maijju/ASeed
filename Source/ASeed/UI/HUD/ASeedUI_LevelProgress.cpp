// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_LevelProgress.h"

void UASeedUI_LevelProgress::NativeOnInitialized()
{
	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ExpBar")));
	LevelLabel = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelLabel")));
}
