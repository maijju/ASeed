// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedEnemy.h"
#include "../Data/ASeedEnemyData.h"
#include "ASeedEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AASeedEnemy::AASeedEnemy()
{
	/*--------------DATA--------------*/
	DataRef = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Enemy/DT_EnemyData.DT_EnemyData'"));

	/*--------------BODY--------------*/
	Body->SetCanEverAffectNavigation(false);

	/*--------------MOVEMENT--------------*/
	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));

	/*--------------CONTROLLER--------------*/
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AASeedEnemyController::StaticClass();

	/*--------------GAS--------------*/
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UASeedEnemyAttributeSet>(TEXT("AttributeSet"));
	ASC->AddAttributeSetSubobject<UASeedEnemyAttributeSet>(AttributeSet);
}

void AASeedEnemy::BeginPlay()
{
	Super::BeginPlay();

	/*--------------LOAD DATA--------------*/
	Data = DataRef.LoadSynchronous();
	FEnemyData* Row = Data->FindRow<FEnemyData>(Key, TEXT(""));
	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load data (%s)"), *Key.ToString());
		return;
	}

	/*--------------CACHE ANIMINST--------------*/
	AnimInst = Cast<UASeedEnemyAnimInst>(MeshComp->GetAnimInstance());

	/*--------------ASSIGN PROPERTIES--------------*/
	ASC->InitAbilityActorInfo(this, this);
	AttributeSet->SetHP(Row->HP);
	AttributeSet->SetHPMax(Row->HPMax);
	AttributeSet->SetAttackRange(Row->AttackRange);
	AttributeSet->SetAttackSpeed(Row->AttackSpeed);
	AttributeSet->SetMoveSpeed(Row->MoveSpeed);
	AnimInst->InitSeqMap(Row->SeqMap);

	/*--------------RUN AI--------------*/
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // Target is always Player
	AAIController* AIController = Cast<AAIController>(GetController());

	if (Player && AIController && BehaviorTree)
	{
		AIController->RunBehaviorTree(BehaviorTree);
		AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Player);
		AIController->GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), Row->AttackRange);
		MovementComp->MaxSpeed = Row->MoveSpeed;
	}
}

void AASeedEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AASeedEnemy::OnDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("OUCH! %f"), AttributeSet->GetHP());
}

void AASeedEnemy::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Dead"));
	Destroy();
}
