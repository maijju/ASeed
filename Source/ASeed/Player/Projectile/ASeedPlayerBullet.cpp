// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerBullet.h"
#include "../../Data/ASeedPlayerData.h"

AASeedPlayerBullet::AASeedPlayerBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	/*--------------BODY--------------*/
	Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	SetRootComponent(Body);
	Body->SetBoxExtent(FVector(60.0, 60.0, 60.0));
	Body->SetCollisionProfileName(TEXT("PlayerAttack"));

	/*--------------MOVEMENT--------------*/
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->SetUpdatedComponent(Body);
	Movement->ProjectileGravityScale = 0.f;
	Movement->InitialSpeed = 2000.f;
	Movement->OnProjectileStop.AddDynamic(this, &AASeedPlayerBullet::ProjectileStop);
}

// Called every frame
void AASeedPlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LifeDuration > 0)
		LifeDuration -= DeltaTime;

	if (LifeDuration <= 0)
	{
		Destroy();
	}
}

void AASeedPlayerBullet::ProjectileStop(const FHitResult& Hit)
{
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerController->GetPawn());

	if (ASI)
	{
		UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();

		if (ASC)
		{
			FGameplayEventData	EventData;
			EventData.Target = Hit.GetActor();
			EventData.Instigator = OwnerController->GetPawn();
			EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.BulletHit"));

			/*--------------------------------------------------*/
			/*----------------HARD CODE WARNING-----------------*/
			/*----------INDEX 0 IS ONLY USE AS EFFECT-----------*/
			/*----------INDEX 1 IS ONLY USE AS CUE--------------*/
			/*--------------------------------------------------*/
			EventData.TargetTags.AddTag(BulletData->GameplayEffectTag);
			EventData.TargetTags.AddTag(BulletData->GameplayBulletHitCueTag);

			FGameplayAbilityTargetData_SingleTargetHit* TargetData =
				new FGameplayAbilityTargetData_SingleTargetHit(Hit);

			EventData.TargetData.Add(TargetData);

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerController->GetPawn(), EventData.EventTag, EventData);
		}
	}
	Destroy();
}

