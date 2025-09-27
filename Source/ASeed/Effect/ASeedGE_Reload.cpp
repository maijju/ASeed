// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_Reload.h"
#include "../AttributeSet/ASeedAttributeSet.h"

UASeedGE_Reload::UASeedGE_Reload()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Info;

	Info.Attribute = UASeedAttributeSet::GetAmmoAttribute();
	Info.ModifierOp = EGameplayModOp::Override;

	FSetByCallerFloat Caller;
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Reload"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	Modifiers.Add(Info);
}
