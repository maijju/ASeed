// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_PlayerRolling.h"
#include "../../Player/ASeedPlayer.h"
#include "../../Data/ASeedPlayerData.h"

UASeedGA_PlayerRolling::UASeedGA_PlayerRolling()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.Skill.Rolling"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	CooldownTag = TEXT("Custom.Cooldown.Rolling");
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(CooldownTag));

	AbilityTriggers.Add(TriggerData);
}

void UASeedGA_PlayerRolling::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	/*---------ROLLING (PHYSICS APPLYING)---------*/
    FVector2D KeyVector = Player->GetCurrentKeyVector();
	FVector RollDirection(KeyVector.Y, KeyVector.X, 0);
	
	FName SectionName;
	if (FMath::IsNearlyEqual(KeyVector.Y, 1.0))
		SectionName = "Forward";
	else if (FMath::IsNearlyEqual(KeyVector.Y, -1.0))
		SectionName = "Backward";
	else if (FMath::IsNearlyEqual(KeyVector.X, 1.0))
		SectionName = "Right";
	else if (FMath::IsNearlyEqual(KeyVector.X, -1.0))
		SectionName = "Left";

	AnimInst->SetRollingSectionName(SectionName);

	// If no key Input, then Rolling to character forward vector
	if (RollDirection.IsNearlyZero())
	{
		RollDirection = Player->GetActorForwardVector();
	}

	RollDirection.Normalize();

	Player->LaunchCharacter(RollDirection * 2000, true, true);
	AnimInst->PlayMontageByType(EMontageType::Rolling);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
