// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_StatUpExpBonus.h"
#include "../../AttributeSet/ASeedAttributeSet.h"

UASeedGE_StatUpExpBonus::UASeedGE_StatUpExpBonus()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UASeedAttributeSet::GetExpBonusAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StatUp.ExpBonus"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
