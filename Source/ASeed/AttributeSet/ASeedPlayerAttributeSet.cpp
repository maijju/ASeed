// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerAttributeSet.h"
#include "../ASeedGameMode.h"
#include "../Player/ASeedPlayer.h"

void UASeedPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetAttackAttribute() ||
		Attribute == GetDefenseAttribute() ||
		Attribute == GetCooldownReduceAttribute() ||
		Attribute == GetAttackSpeedAttribute() ||
		Attribute == GetMoveSpeedAttribute() ||
		Attribute == GetExpBonusAttribute() ||
		Attribute == GetCoreBonusAttribute())
	{
		AASeedGameMode* GM = Cast<AASeedGameMode>(GetWorld()->GetAuthGameMode());
		GM->UpdatedStat(this);
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
			Player->OnDamage(IsDead);
		}
	}
}

void UASeedPlayerAttributeSet::CallbackHPMax()
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (ASC)
	{
		AASeedPlayer* Player = Cast<AASeedPlayer>(ASC->GetAvatarActor());
		if (Player)
		{
			Player->OnHPMaxChanged();
		}
	}
}

void UASeedPlayerAttributeSet::CallbackAmmo()
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (ASC)
	{
		AASeedPlayer* Player = Cast<AASeedPlayer>(ASC->GetAvatarActor());
		if (Player)
		{
			Player->OnAmmoChanged();
		}
	}
}

void UASeedPlayerAttributeSet::CallbackAmmoMax()
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (ASC)
	{
		AASeedPlayer* Player = Cast<AASeedPlayer>(ASC->GetAvatarActor());
		if (Player)
		{
			Player->OnAmmoMaxChanged();
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
