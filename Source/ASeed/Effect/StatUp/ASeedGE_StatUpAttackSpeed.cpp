// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_StatUpAttackSpeed.h"
#include "../../AttributeSet/ASeedAttributeSet.h"

UASeedGE_StatUpAttackSpeed::UASeedGE_StatUpAttackSpeed()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UASeedAttributeSet::GetAttackSpeedAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StatUp.AttackSpeed"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
