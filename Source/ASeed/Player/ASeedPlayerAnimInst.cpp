// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerAnimInst.h"
#include "ASeedPlayer.h"

void UASeedPlayerAnimInst::AnimNotify_AttackL()
{
	AASeedPlayer* PC = Cast<AASeedPlayer>(TryGetPawnOwner());

	if (IsValid(PC))
	{
		PC->Fire(TEXT("Muzzle_02"));
	}
}

void UASeedPlayerAnimInst::AnimNotify_AttackR()
{
	AASeedPlayer* PC = Cast<AASeedPlayer>(TryGetPawnOwner());

	if (IsValid(PC))
	{
		PC->Fire(TEXT("Muzzle_01"));
	}
}