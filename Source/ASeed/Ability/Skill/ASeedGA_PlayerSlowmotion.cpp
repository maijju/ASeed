// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_PlayerSlowmotion.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "../../Player/ASeedPlayer.h"
#include "../../Data/ASeedPlayerData.h"

UASeedGA_PlayerSlowmotion::UASeedGA_PlayerSlowmotion()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.Skill.Slowmotion"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	CooldownTag = TEXT("Custom.Cooldown.Slowmotion");
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(CooldownTag));
}

void UASeedGA_PlayerSlowmotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	/*---------SLOWMOTION---------*/
	UWorld* World = ActorInfo->AvatarActor->GetWorld();
	UGameplayStatics::SetGlobalTimeDilation(World, SlowFactor);
	if (Player)
	{
		Player->CustomTimeDilation = 1.0f / SlowFactor;
		Player->GetVFXComponent()->ActivateVFX(FName("Sandevistan"));
	}

	CachedHandle = Handle;
	CachedActorInfo = ActorInfo;
	CachedActivationInfo = ActivationInfo;

	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, Duration * SlowFactor);
	WaitTask->OnFinish.AddDynamic(this, &UASeedGA_PlayerSlowmotion::EndDuration);
	WaitTask->ReadyForActivation();
}

void UASeedGA_PlayerSlowmotion::EndDuration()
{
	UWorld* World = CachedActorInfo->AvatarActor->GetWorld();
	UE_LOG(LogTemp, Warning, TEXT("Ang"));
	UGameplayStatics::SetGlobalTimeDilation(World, 1.0f);

	AASeedPlayer* Player = Cast<AASeedPlayer>(GetAvatarActorFromActorInfo());
	if (Player)
	{
		Player->CustomTimeDilation = 1.0f;
		Player->GetVFXComponent()->DeactivateVFX();
	}

	EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, false);
}