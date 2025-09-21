// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedEnemy.h"

AASeedEnemy::AASeedEnemy()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UASeedEnemyAttributeSet>(TEXT("AttributeSet"));
	ASC->AddAttributeSetSubobject<UASeedEnemyAttributeSet>(AttributeSet);
}

void AASeedEnemy::BeginPlay()
{
	Super::BeginPlay();

	ASC->InitAbilityActorInfo(this, this);

}

void AASeedEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	AttributeSet->SetHP(100);
	AttributeSet->SetHPMax(100);
}


void AASeedEnemy::OnDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("OUCH"));
}