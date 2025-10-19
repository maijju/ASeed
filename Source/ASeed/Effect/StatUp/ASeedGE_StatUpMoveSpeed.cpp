// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_StatUpMoveSpeed.h"
#include "../../AttributeSet/ASeedAttributeSet.h"

UASeedGE_StatUpMoveSpeed::UASeedGE_StatUpMoveSpeed()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UASeedAttributeSet::GetMoveSpeedAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StatUp.MoveSpeed"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
