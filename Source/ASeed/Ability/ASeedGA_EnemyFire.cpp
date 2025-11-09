// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_EnemyFire.h"
#include "../AttributeSet/ASeedAttributeSet.h"
#include "../Enemy/ASeedEnemyBullet.h"
#include "../Enemy/ASeedEnemy.h"

UASeedGA_EnemyFire::UASeedGA_EnemyFire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Enemy.Fire"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);
}

void UASeedGA_EnemyFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!bActive)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	/*--------------INIT--------------*/
	AASeedEnemy* Enemy = Cast<AASeedEnemy>(ActorInfo->AvatarActor.Get());
	UAbilitySystemComponent* EnemyASC = GetAbilitySystemComponentFromActorInfo();

	if (!Enemy || !EnemyASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const UASeedEnemyBulletData* BulletData = Cast<UASeedEnemyBulletData>(TriggerEventData->OptionalObject);
	if (!BulletData)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletData is nullptr (in ability)"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FVector MuzzleLocation = BulletData->Location;

	/*--------------SPAWN BULLET--------------*/
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AASeedEnemyBullet* Bullet = GetWorld()->SpawnActor<AASeedEnemyBullet>(MuzzleLocation, Enemy->GetActorRotation(), Param);
	Bullet->SetDamage(EnemyASC->GetSet<UASeedAttributeSet>()->GetAttack());
	Bullet->SetBulletData(BulletData);
	Bullet->SetOwnerController(Enemy->GetController());
	Bullet->Body->SetGenerateOverlapEvents(true);

	/*--------------CUE--------------*/
	FGameplayCueParameters CueParam;
	CueParam.Instigator = Enemy;
	CueParam.EffectCauser = Enemy;
	CueParam.Location = MuzzleLocation;
	EnemyASC->ExecuteGameplayCue(BulletData->GameplayMuzzleFlashCueTag, CueParam);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

