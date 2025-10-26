// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_Hit.h"
#include "../AttributeSet/ASeedAttributeSet.h"
#include "../Effect/ASeedGE_Damage.h"
#include "../Effect/ASeedGE_Stun.h"
#include "../Effect/ASeedGE_Burn.h"

UASeedGA_Hit::UASeedGA_Hit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Hit"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

void UASeedGA_Hit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!bActive)
	{
		return;
	}

	else if (!TriggerEventData || !TriggerEventData->Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 전달받은 히트정보를 얻어온다.
	FGameplayAbilityTargetData_SingleTargetHit* HitData =
		static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(TriggerEventData->TargetData.Data[0].Get());

	if (!HitData)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitData Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 이 어빌리티를 발동시킨 Actor를 얻어온다.
	AActor* SourceActor = GetAvatarActorFromActorInfo();

	// 이 어빌리티를 발생시킨 ASC를 얻어온다.
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	// AttributeSet을 얻어온다.
	const UASeedAttributeSet* SourceAttr = SourceASC->GetSet<UASeedAttributeSet>();

	if (!SourceAttr)
	{
		// 활성 어빌리티가 끝날때에는 반드시 EndAbility를 호출해야 한다.
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// 공격처리.
	AActor* TargetActor = HitData->HitResult.GetActor();

	// Target의 ASC를 얻어온다.
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!TargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetASC Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const UASeedAttributeSet* TargetAttr = TargetASC->GetSet<UASeedAttributeSet>();

	if (!TargetAttr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetAttr Failed"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (TargetAttr->GetHP() <= 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	/*--------------EFFECT--------------*/
	for (int32 i = 1; i < TriggerEventData->TargetTags.Num(); ++i)
	{
		FGameplayTag EffectTag = TriggerEventData->TargetTags.GetByIndex(i);
		float EffectDuration = TriggerEventData->EventMagnitude;

		// #0: Dealing
		if (EffectTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Damage"))))
		{
			FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(UASeedGE_Damage::StaticClass(), 1.f);

			float Dmg = SourceAttr->GetAttack() - TargetAttr->GetDefense();
			Dmg = FMath::Max(1.f, Dmg);

			DamageSpecHandle.Data->SetSetByCallerMagnitude(EffectTag, -Dmg);
			SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
		}

		// #1: Stun (Electric Bullet)
		else if (EffectTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Stun"))))
		{
			FGameplayEffectSpecHandle StunSpecHandle = MakeOutgoingGameplayEffectSpec(UASeedGE_Stun::StaticClass(), 1.f);
			StunSpecHandle.Data->SetSetByCallerMagnitude(
				FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.StunDuration")),
				EffectDuration);
			StunSpecHandle.Data->DynamicGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Custom.State.Stun")));
			SourceASC->ApplyGameplayEffectSpecToTarget(*StunSpecHandle.Data, TargetASC);
		}

		// #1: Burn (Inferno Bullet)
		else if (EffectTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Burn"))))
		{
			float TickDmg = (SourceAttr->GetAttack() - TargetAttr->GetDefense() ) / 10;
			TickDmg = FMath::Max(1.f, TickDmg);

			FGameplayEffectSpecHandle BurnSpecHandle = MakeOutgoingGameplayEffectSpec(UASeedGE_Burn::StaticClass(), 1.f);
			BurnSpecHandle.Data->SetSetByCallerMagnitude(
				FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.BurnDuration")),
				EffectDuration);
			BurnSpecHandle.Data->SetSetByCallerMagnitude(
				FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Damage")),
				-TickDmg);
			SourceASC->ApplyGameplayEffectSpecToTarget(*BurnSpecHandle.Data, TargetASC);
		}
	}

	/*--------------CUE--------------*/
	FGameplayCueParameters CueParam;
	FGameplayTag CueTag = TriggerEventData->TargetTags.GetByIndex(0);
	CueParam.Instigator = GetAvatarActorFromActorInfo();
	CueParam.EffectCauser = GetOwningActorFromActorInfo();
	CueParam.Location = HitData->HitResult.ImpactPoint;

	SourceASC->ExecuteGameplayCue(CueTag, CueParam);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
