// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_Title.h"
#include "../ASeedGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UASeedUI_Title::NativeConstruct()
{
	Super::NativeConstruct();

	GI = Cast<UASeedGameInstance>(GetGameInstance());

	Start = Cast<UButton>(GetWidgetFromName(TEXT("Start")));
	Difficulty = Cast<UTextBlock>(GetWidgetFromName(TEXT("Difficulty")));
	DifficultyDown = Cast<UButton>(GetWidgetFromName(TEXT("DifficultyDown")));
	DifficultyUp = Cast<UButton>(GetWidgetFromName(TEXT("DifficultyUp")));
	Exit = Cast<UButton>(GetWidgetFromName(TEXT("Exit")));

	Start->OnClicked.AddDynamic(this, &UASeedUI_Title::StartGame);
	Exit->OnClicked.AddDynamic(this, &UASeedUI_Title::ExitGame);
	DifficultyDown->OnClicked.AddDynamic(this, &UASeedUI_Title::DiffDown);
	DifficultyUp->OnClicked.AddDynamic(this, &UASeedUI_Title::DiffUp);
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

void UASeedUI_Title::DiffDown()
{
	FName Diff = GI->GetDifficulty();

	if (Diff == FName("Easy"))
		return;
	else if (Diff == FName("Normal"))
	{
		GI->SetDifficulty(FName("Easy"));
		Difficulty->SetText(FText::FromString(TEXT("난이도: 쉬움")));
	}
	else if (Diff == FName("Hard"))
	{
		GI->SetDifficulty(FName("Normal"));
		Difficulty->SetText(FText::FromString(TEXT("난이도: 보통")));
	}
}

void UASeedUI_Title::DiffUp()
{
	FName Diff = GI->GetDifficulty();

	if (Diff == FName("Hard"))
		return;
	else if (Diff == FName("Easy"))
	{
		GI->SetDifficulty(FName("Normal"));
		Difficulty->SetText(FText::FromString(TEXT("난이도: 보통")));
	}
	else if (Diff == FName("Normal"))
	{
		GI->SetDifficulty(FName("Hard"));
		Difficulty->SetText(FText::FromString(TEXT("난이도: 어려움")));
	}
}
