// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_StatUpDefense.h"
#include "../../AttributeSet/ASeedAttributeSet.h"

UASeedGE_StatUpDefense::UASeedGE_StatUpDefense()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UASeedAttributeSet::GetDefenseAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StatUp.Defense"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
