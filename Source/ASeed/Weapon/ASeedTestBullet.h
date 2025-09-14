// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"
#include "ASeedTestBullet.generated.h"

UCLASS()
class ASEED_API AASeedTestBullet : public AActor
{
	GENERATED_BODY()

public:
	AASeedTestBullet();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Body;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> Movement;

	TObjectPtr<UParticleSystemComponent> ParticleComp;

	UParticleSystem* HitEffect;
	UParticleSystem* TrailEffect;

	UPROPERTY(EditAnywhere)
	float Life = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ProjectileMovement를 이용하여 움직이는 물체가 다른 물체와 부딪혀서 더이상 움직일
	// 수 없을 때 호출되게 할 함수이다.
	UFUNCTION()
	void ProjectileStop(const FHitResult& Hit);
};