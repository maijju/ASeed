// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "GameplayEffectExtension.h"

#include "../../Data/ASeedPlayerData.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "ASeedPlayerBullet.generated.h"

UCLASS()
class ASEED_API AASeedPlayerBullet : public AActor
{
	GENERATED_BODY()

public:
	AASeedPlayerBullet();

protected:
	/*--------BASIC COMPS--------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Body;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> Movement;

	/*--------PRIMARY DATA--------*/
	UPROPERTY()
	const UASeedPlayerBulletData* BulletData;

	/*--------LOCAL VARIABLES--------*/
	class AController* OwnerController;
	UPROPERTY(EditAnywhere)
	float LifeDuration = 1;
	float Damage = 0;

protected:
	virtual void Tick(float DeltaTime) override;

	void ApplyTrailEffect()
	{
		if (!BulletData || !BulletData->TrailEffect)
			return;
		UGameplayStatics::SpawnEmitterAttached(
			BulletData->TrailEffect,
			RootComponent,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

public:
	void SetDamage(float Amount)
	{
		Damage = Amount;
	}

	void SetOwnerController(class AController* Controller)
	{
		OwnerController = Controller;
	}
	
	void SetBulletData(const UASeedPlayerBulletData* Data)
	{
		BulletData = Data;
		ApplyTrailEffect();
	}

public:
	UFUNCTION()
	void ProjectileStop(const FHitResult& Hit);
};