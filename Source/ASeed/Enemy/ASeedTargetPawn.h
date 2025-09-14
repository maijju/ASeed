// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASeedTargetPawn.generated.h"

UCLASS()
class ASEED_API AASeedTargetPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AASeedTargetPawn();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> Body;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void EnableOutline(bool Enable);
};
