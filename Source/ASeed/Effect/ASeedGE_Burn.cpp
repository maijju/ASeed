#include "ASeedGE_Burn.h"
#include "../AttributeSet/ASeedAttributeSet.h"

UASeedGE_Burn::UASeedGE_Burn()
{
	/*------------DURATION------------*/
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	FSetByCallerFloat DurationCaller;
	DurationCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.BurnDuration"));
	DurationMagnitude = FGameplayEffectModifierMagnitude(DurationCaller);
	Period.Value = 1.0f;

	/*------------DEALING------------*/
	FGameplayModifierInfo DamageInfo;
	DamageInfo.Attribute = UASeedAttributeSet::GetHPAttribute(); 
	DamageInfo.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat DamageCaller;
	DamageCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Damage"));
	DamageInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(DamageCaller);
	Modifiers.Add(DamageInfo);
}
