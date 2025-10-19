// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../ASeedGameMode.h"

AASeedPlayerController::AASeedPlayerController()
{
	bShowMouseCursor = true;
}

void AASeedPlayerController::BeginPlay()
{
	if (IsLocalController())
	{
		MainWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Main.UI_Main_C'"));
		
		if (IsValid(MainWidgetClass))
		{
			MainWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);

			if (MainWidget)
			{
				MainWidget->AddToViewport();
				if (AASeedGameMode* GM = Cast<AASeedGameMode>(GetWorld()->GetAuthGameMode()))
				{
					GM->OnMainWidgetLoaded(MainWidget);
				}
			}
		}
	}
}

