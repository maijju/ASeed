// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedEnemyAnimInst.h"
#include "AIController.h"
#include "ASeedEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

void UASeedEnemyAnimInst::AnimNotify_AttackImpact()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Attacking"));
}

void UASeedEnemyAnimInst::AnimNotify_AttackEnd()
{
	AASeedEnemy* Enemy = Cast<AASeedEnemy>(TryGetPawnOwner());

	if (IsValid(Enemy))
	{
		AAIController* AIController = Enemy->GetController<AAIController>();

		if (IsValid(AIController))
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("AttackEnd"), true);
		}
	}
}
