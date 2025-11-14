// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "ASeedUI_Timer.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedUI_Timer : public UUserWidget
{
	GENERATED_BODY()

protected:
	TObjectPtr<UTextBlock> GameTimer;

protected:
	virtual void NativeOnInitialized();

public:
	void SetTimer(float Time)
	{
		int Minutes = FMath::FloorToInt(Time / 60.f);
		int Seconds = FMath::FloorToInt(FMath::Fmod(Time, 60.f));
		FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		GameTimer->SetText(FText::FromString(TimeString));
	}
};
