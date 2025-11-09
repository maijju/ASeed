// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedPlayerVFXComponent.h"
#include "../ASeedPlayer.h"

UASeedPlayerVFXComponent::UASeedPlayerVFXComponent()
{
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
}

void UASeedPlayerVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AASeedPlayer>(GetOwner());
}

void UASeedPlayerVFXComponent::ActivateVFX(FName VFXName, float Duration)
{
    if (!NiagaraMap.Contains(VFXName))
    {
        UE_LOG(LogTemp, Warning, TEXT("VFX [%s] not found in NiagaraMap"), *VFXName.ToString());
        return;
    }

    UNiagaraSystem* VFXSystem = NiagaraMap[VFXName];
    if (!VFXSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("VFX [%s] system is null"), *VFXName.ToString());
        return;
    }

    UNiagaraComponent* SpawnedComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
        VFXSystem,
        Owner->GetMesh(),
        NAME_None,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget,
        false,   // auto destroy = false
        true
    );

    if (!SpawnedComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Niagara VFX [%s]"), *VFXName.ToString());
        return;
    }

    if (Duration > 0.f)
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateLambda([SpawnedComp]()
                {
                    if (SpawnedComp && SpawnedComp->IsActive())
                    {
                        SpawnedComp->DeactivateImmediate();
                        SpawnedComp->DestroyComponent();
                    }
                }),
            Duration,
            false
        );
    }
}

void UASeedPlayerVFXComponent::DeactivateVFX()
{
    TArray<USceneComponent*> Children;
    Owner->GetComponents(Children);

    for (USceneComponent* Comp : Children)
    {
        if (UNiagaraComponent* Niagara = Cast<UNiagaraComponent>(Comp))
        {
            Niagara->DeactivateImmediate();
            Niagara->DestroyComponent();
        }
    }
}
