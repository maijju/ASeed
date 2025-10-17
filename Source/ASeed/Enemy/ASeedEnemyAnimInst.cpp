// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedEnemyAnimInst.h"
#include "AIController.h"
#include "ASeedEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

void UASeedEnemyAnimInst::NativeInitializeAnimation()
{
	Owner = Cast<AASeedEnemy>(TryGetPawnOwner());
}

void UASeedEnemyAnimInst::AnimNotify_AttackImpact()
{
	if (IsValid(Owner))
	{
		Owner->Attack();
	}
}

void UASeedEnemyAnimInst::AnimNotify_AttackEnd()
{
	if (IsValid(Owner))
	{
		AAIController* AIController = Owner->GetController<AAIController>();

		if (IsValid(AIController))
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("AttackEnd"), true);
		}
	}
}
