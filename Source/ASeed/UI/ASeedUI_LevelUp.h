// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "../Data/ASeedGameData.h"

#include "ASeedUI_LevelUp.generated.h"

/**
 * 
 */
USTRUCT()
struct FCard
{
	GENERATED_BODY()

public:
	TObjectPtr<UButton> Card;
	TObjectPtr<UTextBlock> Stat;
	TObjectPtr<UTextBlock> Desc;
	UPROPERTY()
	EPlayerStats StatEnum;
	UPROPERTY()
	int32 Value;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardSelectedDelegate, FCard, CardIndex);

UCLASS()
class ASEED_API UASeedUI_LevelUp : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnCardSelectedDelegate OnCardSelected;

protected:
	TArray<FCard> Cards;
	
protected:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION()
	void OnCardClicked();

public:
	void InitializeCards(const TArray<FLevelUpRewardInfo>& ShuffleResults);
};
