#include "ASeedGE_Stun.h"

UASeedGE_Stun::UASeedGE_Stun()
{
    DurationPolicy = EGameplayEffectDurationType::HasDuration;
    FSetByCallerFloat DurationCaller;
    DurationCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Custom.Effect.StunDuration"));
    DurationMagnitude = FGameplayEffectModifierMagnitude(DurationCaller);
}
