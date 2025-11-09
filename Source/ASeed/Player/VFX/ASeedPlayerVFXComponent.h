// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

#include "ASeedPlayerVFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASEED_API UASeedPlayerVFXComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UASeedPlayerVFXComponent();

protected:
	TObjectPtr<class AASeedPlayer> Owner;

	TObjectPtr<UNiagaraComponent> NiagaraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UNiagaraSystem*> NiagaraMap;

	FTimerHandle VFXDurationHandle;

protected:
	virtual void BeginPlay() override;

public:
	void ActivateVFX(FName VFXName, float Duration = 0.f);

	UFUNCTION()
	void DeactivateVFX();
};
