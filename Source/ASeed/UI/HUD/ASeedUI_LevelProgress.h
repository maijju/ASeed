// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "ASeedUI_LevelProgress.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedUI_LevelProgress : public UUserWidget
{
	GENERATED_BODY()

protected:
	TObjectPtr<UProgressBar> ExpBar;
	TObjectPtr<UTextBlock> LevelLabel;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void UpdateExpPercent(float CurrentExp, float MaxExp)
	{
		ExpBar->SetPercent(CurrentExp / MaxExp);
	}

	void SetLevelLabel(int32 Level)
	{
		LevelLabel->SetText(FText::AsNumber(Level));
	}
};
