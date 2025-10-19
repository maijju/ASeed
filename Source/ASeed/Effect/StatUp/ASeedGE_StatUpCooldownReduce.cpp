// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_StatUpCooldownReduce.h"
#include "../../AttributeSet/ASeedAttributeSet.h"

UASeedGE_StatUpCooldownReduce::UASeedGE_StatUpCooldownReduce()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UASeedAttributeSet::GetCooldownReduceAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StatUp.CooldownReduce"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
