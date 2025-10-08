// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedBT_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../ASeedEnemy.h"

UASeedBT_Attack::UASeedBT_Attack()
{
	NodeName = TEXT("Attack Task");
	bNotifyTick = true;
	bNotifyTaskFinished = true;

	// 블랙보드에서 사용할 키 설정.
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UASeedBT_Attack, TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UASeedBT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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
		UE_LOG(LogTemp, Warning, TEXT("Target Not Found"));
		return EBTNodeResult::Succeeded;
	}

	AASeedEnemy* Enemy = Cast<AASeedEnemy>(AIController->GetPawn());

	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Me Not Found"));
		return EBTNodeResult::Failed;
	}

	Enemy->ChangeAnimationState(EEnemyAnimState::Attack);

	BlackboardComp->SetValueAsObject(TEXT("AttackTarget"), Target);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UASeedBT_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UASeedBT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 현재 동작되는 AIController를 얻어온다.
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 블랙보드에 지정된 타겟이 없어질 경우 태스크 종료.
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (!AIController || !BlackboardComp)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	AASeedEnemy* Monster = Cast<AASeedEnemy>(AIController->GetPawn());

	if (!Monster)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 공격이 끝났을때 거리를 체크한다.
	bool AttackEnd = BlackboardComp->GetValueAsBool(TEXT("AttackEnd"));

	if (AttackEnd)
	{
		BlackboardComp->SetValueAsBool(TEXT("AttackEnd"), false);

		// 여전히 타겟이 있을 경우 타겟이 공격범위 밖으로 벗어났는지 판단한다.
		float AttackRange = BlackboardComp->GetValueAsFloat(TEXT("AttackRange"));

		FVector	TargetLocation, MonsterLocation;

		// 타겟의 위치를 얻어온다. 단, 발밑 가운데를 기준으로 위치를 지정할 것이기 때문에 캡슐의
		// 절반 높이만큼을 아래로 내린 위치를 구한다.
		TargetLocation = Target->GetActorLocation();

		// 루트컴포넌트가 캡슐일 경우 절반 높이만큼을 아래로 내려준다.
		UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(Target->GetRootComponent());

		if (Capsule)
		{
			TargetLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
		}

		// 몬스터 위치를 구한다.
		MonsterLocation = Monster->GetActorLocation();

		Capsule = Cast<UCapsuleComponent>(Monster->GetRootComponent());

		// 몬스터는 루트컴포넌트가 무조건 Capsule로 정해져 있다.
		MonsterLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();

		// 둘 사이의 거리를 구한다.
		float Distance = FVector::Dist(TargetLocation, MonsterLocation);

		// 둘 사이의 거리가 공격할 거리보다 멀리 있을 경우 공격을 종료한다.
		if (Distance > AttackRange)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		else
		{
			// 공격이 끝날때까지 공격방향을 바라보다가 공격이 끝나면 타겟 방향으로
			// 회전시킨다.
			TargetLocation.Z = 0.0;
			MonsterLocation.Z = 0.0;

			FRotator	Rot = UKismetMathLibrary::FindLookAtRotation(TargetLocation, MonsterLocation);

			Monster->SetActorRotation(Rot);
		}
	}
}

void UASeedBT_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
}
