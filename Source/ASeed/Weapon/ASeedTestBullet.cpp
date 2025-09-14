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

	if (Life > 0)
		Life -= DeltaTime;

	if (Life <= 0)
	{
		Destroy();
	}
		
}

void AASeedTestBullet::ProjectileStop(const FHitResult& Hit)
{
	FDamageEvent	DmgEvent;

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

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());

	USoundBase* Sound = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sound/Fire1.Fire1'"));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, Hit.ImpactPoint);

	Destroy();
}

