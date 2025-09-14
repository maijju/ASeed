// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedTargetPawn.h"

// Sets default values
AASeedTargetPawn::AASeedTargetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));
	SetRootComponent(Body);
	Body->SetCapsuleHalfHeight(92.f);
	Body->SetCapsuleRadius(32.f);
	Body->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComp->SetCollisionProfileName(TEXT("Enemy"));
	MeshComp->bRenderCustomDepth = true;
	MeshComp->SetupAttachment(Body);
	MeshComp->SetRelativeLocation(FVector(0.0, 0.0, -92.0));
}

// Called when the game starts or when spawned
void AASeedTargetPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASeedTargetPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AASeedTargetPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AASeedTargetPawn::EnableOutline(bool Enable)
{
	if (Enable)
	{
		MeshComp->SetCustomDepthStencilValue(1);
	}
	else
	{
		MeshComp->SetCustomDepthStencilValue(0);
	}
	
}

