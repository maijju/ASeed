// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerAnimInst.h"
#include "ASeedPlayer.h"
#include "../ASeedGameMode.h"

void UASeedPlayerAnimInst::NativeInitializeAnimation()
{
	Owner = Cast<AASeedPlayer>(TryGetPawnOwner());
}

void UASeedPlayerAnimInst::AnimNotify_AttackL()
{
	if (IsValid(Owner))
	{
		Owner->Fire(TEXT("Muzzle_02"));
	}
}

void UASeedPlayerAnimInst::AnimNotify_AttackR()
{
	if (IsValid(Owner))
	{
		Owner->Fire(TEXT("Muzzle_01"));
	}
}

void UASeedPlayerAnimInst::AnimNotify_Reloaded()
{
	if (IsValid(Owner))
	{
		Owner->Reloaded();
	}
}

void UASeedPlayerAnimInst::AnimNotify_Installed()
{
	AASeedGameMode* GM = Cast<AASeedGameMode>(GetWorld()->GetAuthGameMode());
	GM->TryInstallModule();
	Owner->ZaWarudo(false);
	Montage_Stop(0.0f);
}

void UASeedPlayerAnimInst::AnimNotify_GameOver()
{
	AASeedGameMode* GM = Cast<AASeedGameMode>(GetWorld()->GetAuthGameMode());
	GM->GameOver();
}
