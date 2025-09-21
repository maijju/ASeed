// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_Cooldown.h"
#include "../AttributeSet/ASeedAttributeSet.h"

UASeedGE_Cooldown::UASeedGE_Cooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat DurationCaller;
	DurationCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Cooldown"));

	DurationMagnitude = FGameplayEffectModifierMagnitude(DurationCaller);
}
