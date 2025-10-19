// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedEnemy.h"
#include "../Data/ASeedEnemyData.h"
#include "../Ability/ASeedGA_Hit.h"
#include "ASeedEnemyController.h"
#include "../ASeedGameMode.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AASeedEnemy::AASeedEnemy()
{
	/*--------------DATA--------------*/
	DataRef = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Enemy/DT_EnemyData.DT_EnemyData'"));

	/*--------------BODY--------------*/
	Body->SetCanEverAffectNavigation(false);

	/*--------------MESH--------------*/
	MeshComp->SetRelativeRotation(FRotator(0, -90, 0));

	/*--------------MOVEMENT--------------*/
	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));

	/*--------------AI--------------*/
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AASeedEnemyController::StaticClass();
	bUseControllerRotationYaw = true;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTAsset(TEXT("/Script/AIModule.BehaviorTree'/Game/Enemy/AI/BT_Enemy.BT_Enemy'"));
	if (BTAsset.Succeeded())
		BehaviorTree = BTAsset.Object;

	/*--------------GAS--------------*/
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UASeedEnemyAttributeSet>(TEXT("AttributeSet"));
	ASC->AddAttributeSetSubobject<UASeedEnemyAttributeSet>(AttributeSet);
}

void AASeedEnemy::BeginPlay()
{
	Super::BeginPlay();

	/*--------------GAS--------------*/
	ASC->InitAbilityActorInfo(this, this);
	ASC->GiveAbility(FGameplayAbilitySpec(UASeedGA_Hit::StaticClass(), 1, INDEX_NONE, this));
	ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Custom.State.Stun")),
		EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AASeedEnemy::OnGameplayStun);

}

void AASeedEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AASeedEnemy::InitializeEnemy(FName EnemyKey)
{
	/*--------------LOAD DATA--------------*/
	Data = DataRef.LoadSynchronous();
	FEnemyData* Row = Data->FindRow<FEnemyData>(EnemyKey, TEXT(""));
	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load data (%s)"), *EnemyKey.ToString());
		return;
	}

	/*--------------APPLYING MESH & ABP--------------*/
	MeshComp->SetSkeletalMesh(Row->Mesh);
	MeshComp->SetAnimInstanceClass(Row->ABP);
	AnimInst = Cast<UASeedEnemyAnimInst>(MeshComp->GetAnimInstance());

	/*--------------ASSIGN PROPERTIES--------------*/
	AttackAbilityTag = Row->AttackAbilityTag;
	AttackEffectTags = Row->AttackEffectTags;
	AttackCueTag = Row->AttackCueTag;
	AttributeSet->SetHP(Row->HP);
	AttributeSet->SetHPMax(Row->HPMax);
	AttributeSet->SetAttack(Row->Attack);
	AttributeSet->SetAttackRange(Row->AttackRange);
	AttributeSet->SetAttackSpeed(Row->AttackSpeed);
	AttributeSet->SetMoveSpeed(Row->MoveSpeed);
	AnimInst->InitSeqMap(Row->SeqMap);
	ExpReward = Row->Exp;
	CreditReward = Row->Gold;

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

void AASeedEnemy::Attack()
{
	if (ASC)
	{
		FVector	Forward = GetActorForwardVector();

		FVector	HitStart = GetActorLocation() + Forward * 50.f;
		FVector	HitEnd = GetActorLocation() + Forward * (50.f + AttributeSet->GetAttackRange());

		FHitResult result;

		FCollisionQueryParams	param;
		param.AddIgnoredActor(this);
		param.bTraceComplex = false;

		bool Collision = GetWorld()->LineTraceSingleByChannel(result, HitStart, HitEnd,
			ECollisionChannel::ECC_Visibility, param);

		//if (Collision)
		//{
		//	DrawDebugLine(
		//		GetWorld(),
		//		HitStart,
		//		result.ImpactPoint,
		//		FColor::Red,
		//		false,
		//		0.5f,
		//		0,
		//		2.0f
		//	);
		//}

		FGameplayEventData	EventData;
		AAIController* AIController = Cast<AAIController>(GetController());
		EventData.Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("AttackTarget")));
		EventData.Instigator = this;
		EventData.EventTag = AttackAbilityTag;

		/*--------------------------------------------------*/
		/*----------------HARD CODE WARNING-----------------*/
		/*----------INDEX 0 IS ONLY USE AS CUE--------------*/
		/*----------INDEX 1 ~ IS ONLY USE AS EFFECT---------*/
		/*--------------------------------------------------*/
		EventData.TargetTags.AddTag(AttackCueTag);

		for (const FGameplayTag Tag : AttackEffectTags)
		{
			EventData.TargetTags.AddTag(Tag);
		}

		FGameplayAbilityTargetData_SingleTargetHit* TargetData =
			new FGameplayAbilityTargetData_SingleTargetHit(result);

		EventData.TargetData.Add(TargetData);

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
	}
}

void AASeedEnemy::OnDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("OUCH! %f"), AttributeSet->GetHP());
}

void AASeedEnemy::OnGameplayStun(const FGameplayTag Tag, int32 Count)
{
	AAIController* AICon = Cast<AAIController>(GetController());
	if (Count > 0)
	{
		AICon->StopMovement();
		AICon->BrainComponent->StopLogic(TEXT("Stun"));
		AnimInst->SetAnimState(EEnemyAnimState::Stun);
	}

	else
	{
		AICon->BrainComponent->RestartLogic();
		AnimInst->SetAnimState(EEnemyAnimState::Idle);
	}
}

void AASeedEnemy::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Dead"));
	AASeedGameMode* GM = Cast<AASeedGameMode>(GetWorld()->GetAuthGameMode());
	GM->EarnEliminationRewards(FRewards(ExpReward, CreditReward));
	Destroy();
}
