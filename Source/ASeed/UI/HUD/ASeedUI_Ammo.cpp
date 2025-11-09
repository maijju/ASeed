// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_Ammo.h"
#include "../../Player/ASeedPlayer.h"

void UASeedUI_Ammo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AmmoText")));

	Player = Cast<AASeedPlayer>(GetOwningPlayerPawn());
	Player->OnUseAmmo.AddDynamic(this, &UASeedUI_Ammo::SetAmmoText);
}