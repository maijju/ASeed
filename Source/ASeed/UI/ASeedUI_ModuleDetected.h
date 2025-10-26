// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "../ASeedGameMode.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "ASeedUI_ModuleDetected.generated.h"

/**
 * 
 */

UCLASS()
class ASEED_API UASeedUI_ModuleDetected : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TObjectPtr<AASeedGameMode> GameMode;
	TObjectPtr<UTextBlock> ModuleName;
	TObjectPtr<UTextBlock> ModuleType;
	TObjectPtr<UTextBlock> ModuleDesc;
	TObjectPtr<UButton> ConfirmButton;

protected:
	virtual void NativeOnInitialized() override;

public:
	void InitializeModule(const FModuleSummary &Module);

};
