// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "ASeedUI_SkillHUD.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedUI_SkillHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	float CoolA;
	float CoolB;
	TObjectPtr<UButton> IconA;
	TObjectPtr<UTextBlock> CoolAText;
	TObjectPtr<UButton> IconB;
	TObjectPtr<UTextBlock> CoolBText;

public:
	bool bFreeze = false;

protected:
	virtual void NativeOnInitialized();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SetCooldownA(float Cool)
	{
		if (CoolA > 0)
			return;

		FButtonStyle Style = IconA->GetStyle();
		Style.Normal.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.6f));
		Style.Hovered.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.6f));
		Style.Pressed.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.6f));
		CoolA = Cool;
	}
	void SetCooldownB(float Cool)
	{
		if (CoolB > 0)
			return;

		FButtonStyle Style = IconB->GetStyle();
		Style.Normal.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.6f));
		Style.Hovered.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.6f));
		Style.Pressed.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 0.6f));
		CoolB = Cool;
	}
};
