// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedEnemyBullet.h"

AASeedEnemyBullet::AASeedEnemyBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	/*--------------BODY--------------*/
	Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	SetRootComponent(Body);
	Body->SetBoxExtent(FVector(60.0, 60.0, 60.0));
	Body->SetCollisionProfileName(TEXT("EnemyAttack"));
	Body->OnComponentBeginOverlap.AddDynamic(this, &AASeedEnemyBullet::OnProjectileHit);
	Body->SetGenerateOverlapEvents(false);

	/*--------------MESH--------------*/
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(Body);
	MeshComp->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	/*--------------MOVEMENT--------------*/
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	MoveComp->SetUpdatedComponent(Body);
	MoveComp->ProjectileGravityScale = 0.f;
}

// Called every frame
void AASeedEnemyBullet::Tick(float DeltaTime)
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

void AASeedEnemyBullet::OnProjectileHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
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
			//Deactivate();
			Destroy();
		}
	}
}

