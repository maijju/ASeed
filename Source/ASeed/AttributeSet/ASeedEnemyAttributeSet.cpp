// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedEnemyAttributeSet.h"
#include "../Enemy/ASeedEnemy.h"

void UASeedEnemyAttributeSet::CallbackHP()
{
	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (ASC)
	{
		// Player를 얻어온다.
		AASeedEnemy* Enemy = Cast<AASeedEnemy>(ASC->GetAvatarActor());

		if (Enemy)
		{
			Enemy->OnDamage();
		}
	}
}