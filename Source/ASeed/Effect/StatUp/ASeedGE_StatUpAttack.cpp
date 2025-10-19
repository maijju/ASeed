// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_StatUpAttack.h"
#include "../../AttributeSet/ASeedAttributeSet.h"

UASeedGE_StatUpAttack::UASeedGE_StatUpAttack()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UASeedAttributeSet::GetAttackAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StatUp.Attack"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
