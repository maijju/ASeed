// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_Victory.h"

void UASeedUI_Victory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Victory = Cast<UTextBlock>(GetWidgetFromName(TEXT("Victory")));
	QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("Quit")));

	QuitButton->OnClicked.AddDynamic(this, &UASeedUI_Victory::QuitGame);
}