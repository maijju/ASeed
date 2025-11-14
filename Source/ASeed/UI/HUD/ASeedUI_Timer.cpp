// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_Timer.h"

void UASeedUI_Timer::NativeOnInitialized()
{
	GameTimer = Cast<UTextBlock>(GetWidgetFromName(TEXT("GameTimer")));
}
