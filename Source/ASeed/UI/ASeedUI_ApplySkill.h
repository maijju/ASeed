// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "../ASeedGameMode.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "ASeedUI_ApplySkill.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedUI_ApplySkill : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TObjectPtr<AASeedGameMode> GameMode;

	TObjectPtr<UTextBlock> CurrentModuleNameA;
	TObjectPtr<UTextBlock> CurrentModuleDescA;
	TObjectPtr<UTextBlock> CurrentModuleNameB;
	TObjectPtr<UTextBlock> CurrentModuleDescB;

	TObjectPtr<UTextBlock> DetectedModuleName;
	TObjectPtr<UTextBlock> DetectedModuleDesc;

	TObjectPtr<UButton> ApplyAButton;
	TObjectPtr<UButton> ApplyBButton;
	TObjectPtr<UButton> DiscardButton;

protected:
	virtual void NativeOnInitialized();

public:
	void InitializedModules(const FModuleSummary& CurrentModuleA, const FModuleSummary& CurrentModuleB, const FModuleSummary& DetectedModule);
};
