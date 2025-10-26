// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/ProgressBar.h"

#include "ASeedUI_HPBar.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedUI_HPBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProgressBar> HPBar;
	float VisibleTime = 0;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();

public:
	void EnableWidget()
	{
		SetVisibility(ESlateVisibility::Visible);
		VisibleTime = 2.f;
	}
	void SetHPPercent(float CurrentHP, float MaxHP)
	{
		if (HPBar)
		{
			EnableWidget();
			HPBar->SetPercent(CurrentHP / MaxHP);
		}
	}
	
};
