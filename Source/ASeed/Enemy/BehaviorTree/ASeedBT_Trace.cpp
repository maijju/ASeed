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

	// ������忡�� ����� Ű ����.
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UASeedBT_Trace, TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UASeedBT_Trace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ���� ���۵Ǵ� AIController�� ���´�.
	AAIController* AIController = OwnerComp.GetAIOwner();

	// ������� �������͸� ���´�.
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

	// ��ã�⿡ �����ߴ��� �Ǵ��Ѵ�.
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

	// ���� �� ���������� �̵� ���� üũ
	EPathFollowingStatus::Type PathStatus = AIController->GetMoveStatus();
	if (PathStatus == EPathFollowingStatus::Idle)
	{
		// ���� ��Ÿ� ���ε� Idle �� ��ã�� ����
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}


void UASeedBT_Trace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	AIController->StopMovement();
}
