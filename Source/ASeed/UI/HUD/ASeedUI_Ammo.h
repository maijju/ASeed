// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ASeedUI_Ammo.generated.h"

/**
 * 
 */
UCLASS()
class ASEED_API UASeedUI_Ammo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TObjectPtr<class AASeedPlayer> Player;
	TObjectPtr<UTextBlock> AmmoText;

protected:
	virtual void NativeOnInitialized();
	
public:
	UFUNCTION()
	void SetAmmoText(float CurrentAmmo, float MaxAmmo)
	{
		if (AmmoText)
		{
			AmmoText->SetText(FText::Format(
				FText::FromString(TEXT("{0} / {1}")),
				FText::AsNumber((int32)CurrentAmmo),
				FText::AsNumber((int32)MaxAmmo)
			));
		}
	}
	
};
