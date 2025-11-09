// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_BuffSpeed.h"
#include "../AttributeSet/ASeedAttributeSet.h"

UASeedGE_BuffSpeed::UASeedGE_BuffSpeed()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat DurationCaller;
	DurationCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.BoostDuration"));

	DurationMagnitude = FGameplayEffectModifierMagnitude(DurationCaller);

	FGameplayModifierInfo Info1;
	Info1.Attribute = UASeedAttributeSet::GetMoveSpeedAttribute();
	Info1.ModifierOp = EGameplayModOp::Multiplicitive;

	FGameplayModifierInfo Info2;
	Info2.Attribute = UASeedAttributeSet::GetAttackSpeedAttribute();
	Info2.ModifierOp = EGameplayModOp::Multiplicitive;

	FSetByCallerFloat Caller;

	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Boost"));
	Info1.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);
	Info2.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	// 모디파이어 배열에 추가한다.
	Modifiers.Add(Info1);
	Modifiers.Add(Info2);
}
