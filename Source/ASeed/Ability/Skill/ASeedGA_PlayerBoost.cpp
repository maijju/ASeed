// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_PlayerBoost.h"
#include "../../Player/ASeedPlayer.h"
#include "../../AttributeSet/ASeedAttributeSet.h"
#include "../../Effect/ASeedGE_BuffSpeed.h"
#include "../../Data/ASeedPlayerData.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UASeedGA_PlayerBoost::UASeedGA_PlayerBoost()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.Skill.Boost"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	CooldownTag = TEXT("Custom.Cooldown.Boost");
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(CooldownTag));
}

void UASeedGA_PlayerBoost::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	/*---------SET COST FIRST!!---------*/
	const UASeedPlayerSkillCost* SkillCost = Cast<UASeedPlayerSkillCost>(TriggerEventData->OptionalObject);
	SetCooldown(SkillCost->Cooldown);
	AddAbilityCost(ECostType::Ammo, SkillCost->AmmoCost);

	/*---------AFTER CALL SUPER---------*/
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!bActive)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	/*---------BOOST---------*/
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	const UASeedAttributeSet* SourceAttr = SourceASC->GetSet<UASeedAttributeSet>();

	if (!SourceAttr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FGameplayEffectSpecHandle BuffSpecHandle =
		MakeOutgoingGameplayEffectSpec(UASeedGE_BuffSpeed::StaticClass(), 1.f);
	
	BuffSpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.BoostDuration")),
		Duration);

	BuffSpecHandle.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(TEXT("Custom.Effect.Boost")),
		SpeedBonus);

	SourceASC->ApplyGameplayEffectSpecToSelf(*BuffSpecHandle.Data.Get());

	AASeedPlayer* Player = Cast<AASeedPlayer>(GetAvatarActorFromActorInfo());
	Player->GetVFXComponent()->ActivateVFX(FName("Boost"));

	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, Duration);
	WaitTask->OnFinish.AddDynamic(this, &UASeedGA_PlayerBoost::EndDuration);
	WaitTask->ReadyForActivation();
}

void UASeedGA_PlayerBoost::EndDuration()
{
	AASeedPlayer* Player = Cast<AASeedPlayer>(GetAvatarActorFromActorInfo());
	if (Player)
	{
		Player->GetVFXComponent()->DeactivateVFX();
	}

	EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, false);
}
