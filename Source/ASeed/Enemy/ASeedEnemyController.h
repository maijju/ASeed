// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ASeedEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API AASeedEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AASeedEnemyController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void StopMovement() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
};
