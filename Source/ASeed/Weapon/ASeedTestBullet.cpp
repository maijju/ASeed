// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedTestBullet.h"

AASeedTestBullet::AASeedTestBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	Movement->OnProjectileStop.AddDynamic(this, &AASeedTestBullet::ProjectileStop);

	/*--------------EFFECT--------------*/
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);
	ParticleComp->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		Hit(TEXT("/Script/Engine.ParticleSystem'/Game/FX/Particles/Abilities/Primary/FX/P_TwinBlast_Primary_HitCharacter.P_TwinBlast_Primary_HitCharacter'"));
	if (Hit.Succeeded())
		HitEffect = Hit.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		Trail(TEXT("/Script/Engine.ParticleSystem'/Game/FX/Particles/Abilities/Primary/FX/P_TwinBlast_Primary_Trail.P_TwinBlast_Primary_Trail'"));
	if (Trail.Succeeded())
		TrailEffect = Trail.Object;
}

// Called when the game starts or when spawned
void AASeedTestBullet::BeginPlay()
{
	Super::BeginPlay();

	if (TrailEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(
			TrailEffect,
			RootComponent,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
}

// Called every frame
void AASeedTestBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LifeDuration > 0)
		LifeDuration -= DeltaTime;

	if (LifeDuration <= 0)
	{
		Destroy();
	}
		
}

void AASeedTestBullet::ProjectileStop(const FHitResult& Hit)
{
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerController->GetPawn());

	if (ASI)
	{
		UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();

		if (ASC)
		{
			FGameplayEventData	EventData;

			// 타겟을 지정한다.
			EventData.Target = Hit.GetActor();

			// 데미지를 가하는 대상을 지정한다.
			EventData.Instigator = OwnerController->GetPawn();

			// 동작시키고자 하는 어빌리티 태그를 지정한다.
			EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("Custom.Player.NormalAttack"));

			// 타겟에 대한 히트 정보를 저장하기 위한 객체를 생성하고 EventData에
			// 지정해준다.
			FGameplayAbilityTargetData_SingleTargetHit* TargetData =
				new FGameplayAbilityTargetData_SingleTargetHit(Hit);

			EventData.TargetData.Add(TargetData);

			// 이벤트를 발생시켜 어빌리티를 실행시킨다.
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerController->GetPawn(), EventData.EventTag, EventData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ASC Failed"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ASI Failed"));
	}

	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitEffect,
			Hit.ImpactPoint,
			Hit.GetActor()->GetActorRotation(),
			FVector(1.3f)
		);
	}

	USoundBase* Sound = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sound/Fire1.Fire1'"));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, Hit.ImpactPoint);

	Destroy();
}

