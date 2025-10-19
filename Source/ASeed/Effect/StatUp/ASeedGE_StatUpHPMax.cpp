// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_StatUpHPMax.h"
#include "../../AttributeSet/ASeedAttributeSet.h"

UASeedGE_StatUpHPMax::UASeedGE_StatUpHPMax()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UASeedAttributeSet::GetHPMaxAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StatUp.HPMax"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
