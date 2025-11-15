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
    Player = Cast<AASeedPlayer>(GetAvatarActorFromActorInfo());
    AnimInst = Player->GetPlayerAnimInstance();
    PlayerASC = GetAbilitySystemComponentFromActorInfo();
    PlayerAttr = PlayerASC->GetSet<UASeedAttributeSet>();

    CachedHandle = Handle;
    CachedActorInfo = ActorInfo;
    CachedActivationInfo = ActivationInfo;

    if (!Player || !PlayerASC)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    /*---------RAMPAGE---------*/
    Player->SetRotationFreeze(true);
    AnimInst->OnMontageEnded.AddDynamic(this, &UASeedGA_PlayerRampage::RepeatFire);
    AnimInst->PlayMontageByType(EMontageType::Rampage);
    bRampaging = true;
}

void UASeedGA_PlayerRampage::RepeatFire(UAnimMontage* Montage, bool bInterrupted)
{
    if (AnimInst->Montage_IsPlaying(Montage))
        return;

    if (PlayerAttr->GetAmmo() > 0 && bRampaging)
    {
        UE_LOG(LogTemp, Warning, TEXT("Firing"))
        AnimInst->PlayMontageByType(EMontageType::Rampage);
    }
    else
    {
        Player->SetRotationFreeze(false);
        bRampaging = false;
        AnimInst->OnMontageEnded.RemoveDynamic(this, &UASeedGA_PlayerRampage::RepeatFire);
        EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, false);
    }
}