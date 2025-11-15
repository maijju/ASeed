// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Components/Button.h"

#include "ASeedUI_GameOver.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedUI_GameOver : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TObjectPtr<UButton> RestartButton;
	TObjectPtr<UButton> QuitButton;

protected:
	virtual void NativeOnInitialized();

protected:
	UFUNCTION()
	void RestartGame()
	{
		FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
		UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));

		APlayerController* PC = Cast<APlayerController>(GetOwningPlayerPawn()->GetController());
		PC->SetPause(false);
	}

	UFUNCTION()
	void QuitGame()
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
	}
};
