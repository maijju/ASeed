// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_StatUpAmmoMax.h"
#include "../../AttributeSet/ASeedAttributeSet.h"

UASeedGE_StatUpAmmoMax::UASeedGE_StatUpAmmoMax()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;
	Info.Attribute = UASeedAttributeSet::GetAmmoMaxAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat	Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StatUp.AmmoMax"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
