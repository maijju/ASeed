// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_PlayerFire.h"
#include "../AttributeSet/ASeedAttributeSet.h"
#include "../Player/Projectile/ASeedPlayerBullet.h"
#include "../Player/ASeedPlayer.h"

UASeedGA_PlayerFire::UASeedGA_PlayerFire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.Fire"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AddAbilityCost(ECostType::Ammo, 1.f);

	AbilityTriggers.Add(TriggerData);
}

void UASeedGA_PlayerFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!bActive)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	AASeedPlayer* Player = Cast<AASeedPlayer>(ActorInfo->AvatarActor.Get());
	if (Player)
	{
		/*--------------INIT--------------*/
		UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();
		const UASeedPlayerBulletData* BulletData = Cast<UASeedPlayerBulletData>(TriggerEventData->OptionalObject);
		FVector MuzzleLocation;
		if (!BulletData)
		{
			UE_LOG(LogTemp, Warning, TEXT("BulletData is nullptr (in ability)"));
		}
		else
		{
			MuzzleLocation = BulletData->Location;
		}

		/*--------------SPAWN BULLET--------------*/
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AASeedPlayerBullet* Bullet = GetWorld()->SpawnActor<AASeedPlayerBullet>(MuzzleLocation, Player->GetActorRotation(), Param);
		Bullet->SetDamage(PlayerASC->GetSet<UASeedAttributeSet>()->GetAttack());
		Bullet->SetBulletData(BulletData);
		Bullet->SetOwnerController(Player->GetController());

		/*--------------CUE--------------*/
		FGameplayCueParameters CueParam;
		CueParam.Instigator = Player;
		CueParam.EffectCauser = Player;
		CueParam.Location = MuzzleLocation;
		PlayerASC->ExecuteGameplayCue(BulletData->GameplayMuzzleFlashCueTag, CueParam);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

