// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "ASeedUI_PlayerHUD.generated.h"

UCLASS()
class ASEED_API UASeedUI_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	TObjectPtr<class AASeedPlayer> Player;
	TObjectPtr<UProgressBar> HPBar;
	TObjectPtr<UTextBlock> HPText;
	TObjectPtr<UProgressBar> CoreBar;
	TObjectPtr<UTextBlock> CoreText;
	TObjectPtr<UTextBlock> CoreAlert;

protected:
	virtual void NativeOnInitialized();

public:
	UFUNCTION()
	void SetHPPercent(float CurrentHP, float MaxHP)
	{
		if (HPBar)
		{
			HPBar->SetPercent(CurrentHP / MaxHP);
		}

		if (HPText)
		{
			if (HPText)
			{
				HPText->SetText(FText::Format(
					FText::FromString(TEXT("{0} / {1}")),
					FText::AsNumber((int32)CurrentHP),
					FText::AsNumber((int32)MaxHP)
				));
			}
		}
	}

	void SetCorePercent(float CurrentCore, float MaxCore)
	{
		float Percent = CurrentCore / MaxCore;
		Percent = FMath::Clamp(Percent, 0.0f, 1.0f);

		if (CoreBar)
		{
			FLinearColor LowColor = FLinearColor(1.0f, 0.1f, 0.1f, 1.0f);
			FLinearColor MidColor = FLinearColor(1.0f, 0.8f, 0.2f, 1.0f);
			FLinearColor HighColor = FLinearColor(0.1f, 1.0f, 0.1f, 1.0f);

			FLinearColor FillColor;

			if (Percent < 0.5f)
			{
				float T = Percent / 0.5f;
				FillColor = FMath::Lerp(LowColor, MidColor, T);
			}
			else
			{
				float T = (Percent - 0.5f) / 0.5f;
				FillColor = FMath::Lerp(MidColor, HighColor, T);
			}

			CoreBar->SetFillColorAndOpacity(FillColor);
			CoreBar->SetPercent(Percent);
		}

		if (CoreText)
		{
			CoreText->SetText(FText::Format(
				FText::FromString(TEXT("{0} / {1}")),
				FText::AsNumber((int32)CurrentCore),
				FText::AsNumber((int32)MaxCore)
			));
		}

		if (CoreAlert)
		{
			if (Percent >= 1.f)
				CoreAlert->SetText(FText::FromString(TEXT("모듈 설치 가능!")));
			else
				CoreAlert->SetText(FText());
		}
	}
};
