// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerAttributeSet.h"
#include "../Player/ASeedPlayer.h"

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

void UASeedPlayerAttributeSet::CallbackMoveSpeed(float NewMoveSpeed)
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (ASC)
	{
		AASeedPlayer* Player = Cast<AASeedPlayer>(ASC->GetAvatarActor());

		if (Player)
		{
			if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
			{
				MoveComp->MaxWalkSpeed = NewMoveSpeed;
			}
		}
	}
}
