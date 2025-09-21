// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedAttributeSet.h"

void UASeedAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		CallbackAmmo();
	}

	else if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("HP Change Detected"));
		CallbackHP();
	}
}

void UASeedAttributeSet::CallbackAmmo()
{
}

void UASeedAttributeSet::CallbackHP()
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