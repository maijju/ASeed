// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedAttributeSet.h"

void UASeedAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	UE_LOG(LogTemp, Warning, TEXT("Attribute Changed: %s"), *Data.EvaluatedData.Attribute.GetName());
	
	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetHPMax()));

		bool IsDead = GetHP() <= 0.0f ? true : false;

		CallbackHP(IsDead);
	}

	if (Data.EvaluatedData.Attribute == GetHPMaxAttribute())
	{
		CallbackHPMax();
	}
	
	if (Data.EvaluatedData.Attribute == GetAmmoAttribute())
	{
		CallbackAmmo();
	}

	if (Data.EvaluatedData.Attribute == GetAmmoMaxAttribute())
	{
		CallbackAmmoMax();
	}
}

void UASeedAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		CallbackMoveSpeed(GetMoveSpeed());
	}
}

void UASeedAttributeSet::CallbackAmmo()
{
}

void UASeedAttributeSet::CallbackAmmoMax()
{
}

void UASeedAttributeSet::CallbackHP(bool IsDead)
{

}

void UASeedAttributeSet::CallbackHPMax()
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

void UASeedAttributeSet::CallbackMoveSpeed(float NewMoveSpeed)
{

}