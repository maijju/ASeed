// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerBullet.h"
#include "../../Data/ASeedPlayerData.h"

AASeedPlayerBullet::AASeedPlayerBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	/*--------------BODY--------------*/
	Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	SetRootComponent(Body);
	Body->SetBoxExtent(FVector(100.0, 100.0, 100.0));
	Body->SetCollisionProfileName(TEXT("PlayerAttack"));
	Body->OnComponentBeginOverlap.AddDynamic(this, &AASeedPlayerBullet::OnProjectileHit);
	Body->SetGenerateOverlapEvents(false);

	/*--------------MOVEMENT--------------*/
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	MoveComp->SetUpdatedComponent(Body);
	MoveComp->ProjectileGravityScale = 0.f;
	MoveComp->InitialSpeed = 2500.f;
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
		//Deactivate();
	}
}

void AASeedPlayerBullet::OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerController->GetPawn());

	if (ASI)
	{
		UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();

		if (ASC)
		{
			FGameplayEventData	EventData;
			EventData.Target = Hit.GetActor();

			if (Hit.GetActor()->IsA<AASeedPlayerBullet>())
			{
				return;
			}

			EventData.Instigator = OwnerController->GetPawn();
			EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Hit"));

			/*--------------------------------------------------*/
			/*----------------HARD CODE WARNING-----------------*/
			/*----------INDEX 0 IS ONLY USE AS CUE--------------*/
			/*----------INDEX 1 ~ IS ONLY USE AS EFFECT---------*/
			/*--------------------------------------------------*/
			EventData.TargetTags.AddTag(BulletData->GameplayBulletHitCueTag);

			for (const FGameplayTag Tag : BulletData->GameplayEffectTags)
			{
				EventData.TargetTags.AddTag(Tag);
			}

			EventData.EventMagnitude = BulletData->EffectDuration;

			FGameplayAbilityTargetData_SingleTargetHit* TargetData =
				new FGameplayAbilityTargetData_SingleTargetHit(Hit);

			EventData.TargetData.Add(TargetData);

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerController->GetPawn(), EventData.EventTag, EventData);

			PierceCount--;
			if (PierceCount <= 0)
				Destroy();
			//Deactivate();
		}
	}
}

