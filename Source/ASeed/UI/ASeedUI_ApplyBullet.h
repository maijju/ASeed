// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "../ASeedGameMode.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "ASeedUI_ApplyBullet.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedUI_ApplyBullet : public UUserWidget
{
	GENERATED_BODY()

protected:
	TObjectPtr<AASeedGameMode> GameMode;

	TObjectPtr<UTextBlock> CurrentModuleName;
	TObjectPtr<UTextBlock> CurrentModuleDesc;
	TObjectPtr<UTextBlock> DetectedModuleName;
	TObjectPtr<UTextBlock> DetectedModuleDesc;

	TObjectPtr<UButton> ApplyButton;
	TObjectPtr<UButton> DiscardButton;

protected:
	virtual void NativeOnInitialized();

public:
	void InitializedModules(const FModuleSummary& CurrentModule, const FModuleSummary& DetectedModule);
	
};
