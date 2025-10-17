// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Data/ASeedPlayerData.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "GameplayEffectExtension.h"

#include "Components/ActorComponent.h"
#include "ASeedPlayerProjectileComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASEED_API UASeedPlayerProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UASeedPlayerProjectileComponent();

protected:
	/*--------DATATABLE--------*/
	TSoftObjectPtr<UDataTable> BulletDataRef;
	UPROPERTY();
	UDataTable* BulletData;

	TArray<FGameplayTag> GameplayEffectTags;
	FGameplayTag GameplayMuzzleFlashCueTag;
	FGameplayTag GameplayBulletHitCueTag;
	UParticleSystem* TrailEffect;
	float EffectDuration;
	int32 PierceCount = 1;
	
	TObjectPtr<class AASeedPlayer> Owner;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void TryFireBullet(FName SocketName);
	void UpdateBulletDataByKey(FName BulletKey)
	{
		if (!BulletData)
		{
			UE_LOG(LogTemp, Warning, TEXT("BulletData is nullptr"));
		}
		else
		{
			FPlayerBulletData* Row = BulletData->FindRow<FPlayerBulletData>(BulletKey, TEXT(""));

			if (!Row)
				return;

			GameplayEffectTags = Row->GameplayEffectTags;
			GameplayMuzzleFlashCueTag = Row->GameplayMuzzleFlashCueTag;
			GameplayBulletHitCueTag = Row->GameplayBulletHitCueTag;
			TrailEffect = Row->TrailEffect;
			EffectDuration = Row->EffectDuration;
			PierceCount = Row->PierceCount;

		}
	}
};
