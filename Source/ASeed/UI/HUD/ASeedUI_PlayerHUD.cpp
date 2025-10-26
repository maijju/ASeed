// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_PlayerHUD.h"
#include "../../Player/ASeedPlayer.h"

void UASeedUI_PlayerHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	HPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPText")));
	CoreBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("CoreBar")));
	CoreText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoreText")));
	CoreAlert = Cast<UTextBlock>(GetWidgetFromName(TEXT("CoreAlert")));

	Player = Cast<AASeedPlayer>(GetOwningPlayerPawn());
	Player->OnPlayerHit.AddDynamic(this, &UASeedUI_PlayerHUD::SetHPPercent);

	CoreAlert->SetText(FText());
}