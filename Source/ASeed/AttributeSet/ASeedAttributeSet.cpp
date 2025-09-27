// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedAttributeSet.h"

void UASeedAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetAmmoAttribute())
	{
		CallbackAmmo();
	}

	else if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetHPMax()));

		bool IsDead = GetHP() <= 0.0f ? true : false;

		CallbackHP(IsDead);
	}
}

void UASeedAttributeSet::CallbackAmmo()
{
}

void UASeedAttributeSet::CallbackHP(bool IsDead)
{

}

void UASeedAttributeSet::CallbackMP()
{

}

void UASeedAttributeSet::CallbackAttack()
{

}

void UASeedAttributeSet::CallbackDefense()
{

}