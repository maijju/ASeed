// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_StatUpCoreBonus.h"
#include "../../AttributeSet/ASeedAttributeSet.h"

UASeedGE_StatUpCoreBonus::UASeedGE_StatUpCoreBonus()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UASeedAttributeSet::GetCoreBonusAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StatUp.CoreBonus"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
