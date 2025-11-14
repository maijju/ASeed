// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_BossHP.h"

void UASeedUI_BossHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UASeedUI_BossHP::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP")));
}

void UASeedUI_BossHP::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
}