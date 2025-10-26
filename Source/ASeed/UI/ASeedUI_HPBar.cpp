// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_HPBar.h"

void UASeedUI_HPBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Visible)
	{
		VisibleTime -= InDeltaTime;

		if (VisibleTime <= 0.f)
		{
			VisibleTime = 0.f;
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UASeedUI_HPBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP")));
}

void UASeedUI_HPBar::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
}