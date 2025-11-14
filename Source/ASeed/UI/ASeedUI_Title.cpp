// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_Title.h"
#include "Kismet/GameplayStatics.h"

void UASeedUI_Title::NativeConstruct()
{
	Super::NativeConstruct();

	Start = Cast<UButton>(GetWidgetFromName(TEXT("Start")));
	Difficulty = Cast<UTextBlock>(GetWidgetFromName(TEXT("Difficulty")));
	DifficultyDown = Cast<UButton>(GetWidgetFromName(TEXT("DifficultyDown")));
	DifficultyUp = Cast<UButton>(GetWidgetFromName(TEXT("DifficultyUp")));
	Exit = Cast<UButton>(GetWidgetFromName(TEXT("Exit")));

	Start->OnClicked.AddDynamic(this, &UASeedUI_Title::StartGame);
	Exit->OnClicked.AddDynamic(this, &UASeedUI_Title::ExitGame);
}

void UASeedUI_Title::StartGame()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        FInputModeGameAndUI InputMode;
        InputMode.SetHideCursorDuringCapture(false);
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }

    RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));
}

void UASeedUI_Title::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
