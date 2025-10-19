// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ASeedPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API AASeedPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AASeedPlayerController();

protected:
	TSubclassOf<UUserWidget> MainWidgetClass;
	UPROPERTY()
	UUserWidget* MainWidget;

protected:
	virtual void BeginPlay() override;

public:
	UUserWidget* GetMainWidget()
	{
		return MainWidget;
	}
	
};
