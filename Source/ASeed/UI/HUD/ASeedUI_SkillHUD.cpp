// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_SkillHUD.h"

void UASeedUI_SkillHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	IconA = Cast<UButton>(GetWidgetFromName(TEXT("IconA")));
	CoolAText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoolAText")));
	IconB = Cast<UButton>(GetWidgetFromName(TEXT("IconB")));
	CoolBText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoolBText")));

    CoolAText->SetText(FText::GetEmpty());
    CoolBText->SetText(FText::GetEmpty());
}

void UASeedUI_SkillHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    if (bFreeze)
        return;

    if (CoolA > 0)
    {
        CoolA -= InDeltaTime;
        int32 CoolAInt = FMath::FloorToInt(CoolA);
        CoolAText->SetText(FText::AsNumber(CoolAInt));
    }
    else
    {
        CoolAText->SetText(FText::GetEmpty());
        FButtonStyle Style = IconA->GetStyle();
        Style.Normal.TintColor = FSlateColor(FLinearColor(0.f, 0.5f, 0.5f, 1.f));
        Style.Hovered.TintColor = FSlateColor(FLinearColor(0.f, 0.5f, 0.5f, 1.f));
        Style.Pressed.TintColor = FSlateColor(FLinearColor(0.f, 0.5f, 0.5f, 1.f));
    }

    if (CoolB > 0)
    {
        CoolB -= InDeltaTime;
        int32 CoolBInt = FMath::FloorToInt(CoolB);
        CoolBText->SetText(FText::AsNumber(CoolBInt));
    }
    else
    {
        CoolBText->SetText(FText::GetEmpty());
        FButtonStyle Style = IconB->GetStyle();
        Style.Normal.TintColor = FSlateColor(FLinearColor(0.f, 0.5f, 0.5f, 1.f));
        Style.Hovered.TintColor = FSlateColor(FLinearColor(0.f, 0.5f, 0.5f, 1.f));
        Style.Pressed.TintColor = FSlateColor(FLinearColor(0.f, 0.5f, 0.5f, 1.f));
    }
}

