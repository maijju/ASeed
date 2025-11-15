// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "ASeedUI_Victory.generated.h"

/**
 *
 */
UCLASS()
class ASEED_API UASeedUI_Victory : public UUserWidget
{
	GENERATED_BODY()

public:
	TObjectPtr<UTextBlock> Victory;

protected:
	TObjectPtr<UButton> QuitButton;

protected:
	virtual void NativeOnInitialized();

protected:
	UFUNCTION()
	void QuitGame()
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
	}
};
