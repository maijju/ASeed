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

	// ������忡�� ����� Ű ����.
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UASeedBT_Attack, TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UASeedBT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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
	// ���� ���۵Ǵ� AIController�� ���´�.
	AAIController* AIController = OwnerComp.GetAIOwner();

	// ������忡 ������ Ÿ���� ������ ��� �½�ũ ����.
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

	// ������ �������� �Ÿ� üũ
	bool AttackEnd = BlackboardComp->GetValueAsBool(TEXT("AttackEnd"));

	if (AttackEnd)
	{
		BlackboardComp->SetValueAsBool(TEXT("AttackEnd"), false);

		float AttackRange = BlackboardComp->GetValueAsFloat(TEXT("AttackRange"));

		FVector	TargetLocation, MonsterLocation;
		TargetLocation = Target->GetActorLocation();

		UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(Target->GetRootComponent());

		if (Capsule)
		{
			TargetLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
		}

		MonsterLocation = Monster->GetActorLocation();
		Capsule = Cast<UCapsuleComponent>(Monster->GetRootComponent());
		MonsterLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();

		// ���� �÷��̾� ������ �Ÿ�
		float Distance = FVector::Dist(TargetLocation, MonsterLocation);

		// �� ������ �Ÿ��� ������ �Ÿ����� �ָ� ���� ��� ������ �����Ѵ�.
		if (Distance > AttackRange)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		else
		{
		
			TargetLocation.Z = 0.0;
			MonsterLocation.Z = 0.0;

			FRotator Rot = UKismetMathLibrary::FindLookAtRotation(MonsterLocation, TargetLocation);

			Monster->SetActorRotation(Rot);
		}
	}
}

void UASeedBT_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
}
