// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_GameOver.h"

void UASeedUI_GameOver::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RestartButton = Cast<UButton>(GetWidgetFromName(TEXT("Restart")));
	QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("Quit")));

	RestartButton->OnClicked.AddDynamic(this, &UASeedUI_GameOver::RestartGame);
}