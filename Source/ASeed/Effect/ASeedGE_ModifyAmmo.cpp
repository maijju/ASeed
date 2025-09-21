// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGE_ModifyAmmo.h"
#include "../AttributeSet/ASeedAttributeSet.h"

UASeedGE_ModifyAmmo::UASeedGE_ModifyAmmo()
{
	/*
	Instant : 즉시발동형. 효과가 바로 적용된다. Duration을 무시한다.
	Infinite : 무한 지속형. 한번 적용하면 제거될 때까지 계속 유지된다. 제거하려면
	GameplayEffect의 Removal 이나 만료 조건을 작성해야 한다.
	HasDuration : 지속형. 지속시간동안 유지된다. Duration 변수에 지속시간을 지정한다.
	*/
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 지속시간이 있을 경우 지속시간을 이렇게 지정한다.
	//DurationMagnitude = FScalableFloat(2.f);

	// 데미지를 가하면 체력이 감소되어야 한다.
	// 감소될 체력이 고정값이면 FScalableFloat을 이용하여 고정값을 지정하면 되지만 유동적인
	// 값이라면 SetByCaller를 이용해서 변경되는 값을 지정할 수 있도록 해야 한다.
	FGameplayModifierInfo	Info;

	// 변경하고자 하는 속성을 지정한다. 속성은 일반 값이 아닌 HP속성을 의미한다.
	Info.Attribute = UASeedAttributeSet::GetHPAttribute();

	/*
	Additive : Attribute에 지정된 속성의 값에 지정된 값을 더해준다.
	Multiplicitive : Attribute에 지정된 속성의 값에 지정된 값을 곱해준다.
	Division : Attribute에 지정된 속성의 값에 지정된 값을 나눠준다.
	Override : Attribute에 지정된 속성의 값에 지정된 값을 덮어쓴다.
	*/
	Info.ModifierOp = EGameplayModOp::Additive;

	// SetByCaller를 만들어서 지정한다.
	// 값이 고정일 경우 아래와 같이 지정한다.
	//Info.ModifierMagnitude = FScalableFloat(100.f);

	FSetByCallerFloat	Caller;

	// 아래에서 지정한 Tag로 변경된 값을 넘겨서 동작시켜줄 수 있게 된다.
	Caller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.ModifyAmmo"));
	Info.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller);

	// 모디파이어 배열에 추가한다.
	Modifiers.Add(Info);

	/*FGameplayEffectCue	Cue;

	Cue.GameplayCueTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Custom.Cue.WarriorDamage")));

	GameplayCues.Add(Cue);*/
}
