// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_PlayerRampage.h"
#include "../../Player/ASeedPlayer.h"
#include "../../Data/ASeedPlayerData.h"

UASeedGA_PlayerRampage::UASeedGA_PlayerRampage()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

    FAbilityTriggerData	TriggerData;
    TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.Skill.Rampage"));
    TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
    AbilityTriggers.Add(TriggerData);

    CooldownTag = TEXT("Custom.Cooldown.Rampage");
    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(CooldownTag));
}

void UASeedGA_PlayerRampage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

    /*---------INIT---------*/
    AASeedPlayer* Player = Cast<AASeedPlayer>(GetAvatarActorFromActorInfo());
    UASeedPlayerAnimInst* AnimInst = Player->GetPlayerAnimInstance();
    UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();

    if (!Player || !PlayerASC)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    /*---------RAMPAGE---------*/
    Player->SetRotationFreeze(true);
    AnimInst->PlayMontageByType(EMontageType::Rampage);

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
