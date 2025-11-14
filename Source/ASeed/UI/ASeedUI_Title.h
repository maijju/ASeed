// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "ASeedUI_Title.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedUI_Title : public UUserWidget
{
	GENERATED_BODY()
	
	TObjectPtr<UButton> Start;
	TObjectPtr<UTextBlock> Difficulty;
	TObjectPtr<UButton> DifficultyDown;
	TObjectPtr<UButton> DifficultyUp;
	TObjectPtr<UButton> Exit;

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void ExitGame();
};
