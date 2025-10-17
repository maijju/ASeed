// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerProjectileComponent.h"
#include "../ASeedPlayer.h"
#include "../../Ability/ASeedGA_PlayerFire.h"

// Sets default values for this component's properties
UASeedPlayerProjectileComponent::UASeedPlayerProjectileComponent()
{
	BulletDataRef = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/DT_PlayerBullet.DT_PlayerBullet'"));
}


// Called when the game starts
void UASeedPlayerProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	BulletData = BulletDataRef.LoadSynchronous();
	Owner = Cast<AASeedPlayer>(GetOwner());
}

void UASeedPlayerProjectileComponent::TryFireBullet(FName SocketName)
{
	if (!Owner)
		return;

	UAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();

	if (ASC)
	{
		TObjectPtr<UASeedPlayerBulletData> BulletPayload = NewObject<UASeedPlayerBulletData>();
		BulletPayload->Location = Owner->GetMesh()->GetSocketLocation(SocketName);
		BulletPayload->GameplayEffectTags = GameplayEffectTags;
		BulletPayload->GameplayMuzzleFlashCueTag = GameplayMuzzleFlashCueTag;
		BulletPayload->GameplayBulletHitCueTag = GameplayBulletHitCueTag;
		BulletPayload->TrailEffect = TrailEffect;
		BulletPayload->EffectDuration = EffectDuration;
		BulletPayload->PierceCount = PierceCount;

		FGameplayEventData EventData;
		EventData.Instigator = Owner;
		EventData.OptionalObject = BulletPayload;
		ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.Fire")), &EventData);
	}

}



