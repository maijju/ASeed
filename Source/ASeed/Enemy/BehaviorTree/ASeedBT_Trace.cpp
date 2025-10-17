// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedBT_Trace.h"
#include "AIController.h"
#include "../ASeedEnemy.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UASeedBT_Trace::UASeedBT_Trace()
{
	NodeName = TEXT("Trace Task");
	bNotifyTick = true;
	bNotifyTaskFinished = true;

	// 블랙보드에서 사용할 키 설정.
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UASeedBT_Trace, TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UASeedBT_Trace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 현재 동작되는 AIController를 얻어온다.
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 블랙보드 컴포넌터를 얻어온다.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Failed"));
		return EBTNodeResult::Succeeded;
	}

	EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(Target);

	// 길찾기에 성공했는지 판단한다.
	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}

	AASeedEnemy* Enemy = Cast<AASeedEnemy>(AIController->GetPawn());

	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	Enemy->ChangeAnimationState(EEnemyAnimState::Run);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UASeedBT_Trace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController)
	{
		AIController->StopMovement();
	}

	return EBTNodeResult::Aborted;
}

void UASeedBT_Trace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AASeedEnemy* Enemy = Cast<AASeedEnemy>(AIController->GetPawn());
	if (!Enemy)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	float AttackRange = BlackboardComp->GetValueAsFloat(TEXT("AttackRange"));
	float Distance = FVector::Dist(Target->GetActorLocation(), Enemy->GetActorLocation());

	if (Distance <= AttackRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 아직 못 도착했으면 이동 상태 체크
	EPathFollowingStatus::Type PathStatus = AIController->GetMoveStatus();
	if (PathStatus == EPathFollowingStatus::Idle)
	{
		// 공격 사거리 밖인데 Idle → 길찾기 실패
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}


void UASeedBT_Trace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	AIController->StopMovement();
}
