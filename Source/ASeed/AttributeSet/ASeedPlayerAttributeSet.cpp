// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerAttributeSet.h"
#include "../Player/ASeedPlayer.h"

void UASeedPlayerAttributeSet::CallbackAmmo()
{
	// ASC를 얻어온다.
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (ASC)
	{
		// Player를 얻어온다.
		AASeedPlayer* Player = Cast<AASeedPlayer>(ASC->GetAvatarActor());

		if (Player)
		{
			Player->OnAmmoModified();
		}
	}
}

void UASeedPlayerAttributeSet::CallbackHP(bool IsDead)
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (ASC)
	{
		AASeedPlayer* Player = Cast<AASeedPlayer>(ASC->GetAvatarActor());

		if (Player)
		{
			if (!IsDead)
			{
				Player->OnDamage();
			}
			else
			{
				Player->Die();
			}
			
		}
	}
}

void UASeedPlayerAttributeSet::CallbackAttack()
{

}

void UASeedPlayerAttributeSet::CallbackDefense()
{

}