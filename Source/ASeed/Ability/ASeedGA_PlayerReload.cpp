// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_PlayerReload.h"
#include "../AttributeSet/ASeedAttributeSet.h"
#include "../Effect/ASeedGE_Reload.h"

UASeedGA_PlayerReload::UASeedGA_PlayerReload()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.Reload"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

void UASeedGA_PlayerReload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!bActive)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();
	const UASeedAttributeSet* PlayerAttr = PlayerASC->GetSet<UASeedAttributeSet>();
	FGameplayEffectSpecHandle SpecHandle = PlayerASC->MakeOutgoingSpec(UASeedGE_Reload::StaticClass(), 1.0f, PlayerASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		float AmmoMax = PlayerAttr->GetAmmoMax();

		SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Reload")),
			AmmoMax
		);

		PlayerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}