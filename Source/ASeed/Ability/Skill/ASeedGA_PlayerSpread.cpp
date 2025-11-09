// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedGA_PlayerSpread.h"
#include "../../Player/ASeedPlayer.h"
#include "../../Data/ASeedPlayerData.h"

UASeedGA_PlayerSpread::UASeedGA_PlayerSpread()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	FAbilityTriggerData	TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.Skill.Spread"));
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);

	CooldownTag = TEXT("Custom.Cooldown.Spread");
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(CooldownTag));
}

void UASeedGA_PlayerSpread::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

    const FPlayerBulletData* BulletData = Player->GetProjectileComponent()->GetBulletData();
    if (!BulletData)
    {
        UE_LOG(LogTemp, Warning, TEXT("BulletData is nullptr in MultiFire ability"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    TObjectPtr<UASeedPlayerBulletData> BulletPayload = NewObject<UASeedPlayerBulletData>();
    BulletPayload->GameplayEffectTags = BulletData->GameplayEffectTags;
    BulletPayload->GameplayMuzzleFlashCueTag = BulletData->GameplayMuzzleFlashCueTag;
    BulletPayload->GameplayBulletHitCueTag = BulletData->GameplayBulletHitCueTag;
    BulletPayload->TrailEffect = BulletData->TrailEffect;
    BulletPayload->EffectDuration = BulletData->EffectDuration;
    BulletPayload->PierceCount = BulletData->PierceCount;

    FVector MuzzleLocation = Player->GetActorLocation();

	/*---------SPREAD---------*/
    FRotator BaseRotation = Player->GetActorRotation();

    TArray<FVector> Directions = {
        FVector::ForwardVector,
        -FVector::ForwardVector,
        FVector::RightVector,
        -FVector::RightVector,
        (FVector::ForwardVector + FVector::RightVector).GetSafeNormal(),
        (FVector::ForwardVector - FVector::RightVector).GetSafeNormal(),
        (-FVector::ForwardVector + FVector::RightVector).GetSafeNormal(),
        (-FVector::ForwardVector - FVector::RightVector).GetSafeNormal()
    };

    FActorSpawnParameters Param;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (const FVector& Dir : Directions)
    {
        FVector FinalDir = BaseRotation.RotateVector(Dir);
        FRotator BulletRot = FinalDir.Rotation();

        AASeedPlayerBullet* Bullet = GetWorld()->SpawnActor<AASeedPlayerBullet>(
            MuzzleLocation, BulletRot, Param);

        if (Bullet)
        {
            Bullet->SetDamage(PlayerASC->GetSet<UASeedAttributeSet>()->GetAttack());
            Bullet->SetBulletData(BulletPayload);
            Bullet->SetOwnerController(Player->GetController());
            Bullet->Body->SetGenerateOverlapEvents(true);
        }
    }
    AnimInst->PlayMontageByType(EMontageType::Spread);

    /*---------CUE---------*/
    FGameplayCueParameters CueParam;
    CueParam.Instigator = Player;
    CueParam.EffectCauser = Player;
    CueParam.Location = MuzzleLocation;
    PlayerASC->ExecuteGameplayCue(BulletData->GameplayMuzzleFlashCueTag, CueParam);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
