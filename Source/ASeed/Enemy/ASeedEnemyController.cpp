#include "ASeedEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

AASeedEnemyController::AASeedEnemyController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AASeedEnemyController::BeginPlay()
{
    Super::BeginPlay();
}

void AASeedEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AASeedEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AASeedEnemyController::StopMovement()
{
    Super::StopMovement();
}

void AASeedEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);
    if (Result.IsSuccess()) {
        UE_LOG(LogTemp, Log, TEXT("OnMoveCompleted: success"));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("OnMoveCompleted: failed/aborted - %s"), *Result.ToString());
    }
}
