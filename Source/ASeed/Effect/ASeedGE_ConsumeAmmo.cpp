// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_ConsumeAmmo.h"
#include "../AttributeSet/ASeedAttributeSet.h"

UASeedGE_ConsumeAmmo::UASeedGE_ConsumeAmmo()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo	Info;

	Info.Attribute = UASeedAttributeSet::GetAmmoAttribute();
	Info.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat Caller;
	// 아래에서 지정한 Tag로 변경된 값을 넘겨서 동작시켜줄 수 있게 된다.
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.ConsumeAmmo"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	// 모디파이어 배열에 추가한다.
	Modifiers.Add(Info);
}
