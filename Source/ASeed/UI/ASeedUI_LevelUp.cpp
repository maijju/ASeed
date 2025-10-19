// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_LevelUp.h"

void UASeedUI_LevelUp::NativeOnInitialized()
{
	Cards.SetNum(3);

	for (int32 i=0; i<3; i++)
	{
		FString CardId = FString::Printf(TEXT("Card%d"), i + 1);
		FString StatId = FString::Printf(TEXT("Stat%d"), i + 1);
		FString DescId = FString::Printf(TEXT("Desc%d"), i + 1);
		Cards[i].Card = Cast<UButton>(GetWidgetFromName(*CardId));
		Cards[i].Card->OnClicked.AddDynamic(this, &UASeedUI_LevelUp::OnCardClicked);
		Cards[i].Stat = Cast<UTextBlock>(GetWidgetFromName(*StatId));
		Cards[i].Desc = Cast<UTextBlock>(GetWidgetFromName(*DescId));
	}
}

void UASeedUI_LevelUp::OnCardClicked()
{
	for (int32 i=0; i<3; i++)
	{
		if (Cards[i].Card->IsHovered() || Cards[i].Card->IsPressed())
		{
			FCard SelectedCard;
			SelectedCard.StatEnum = Cards[i].StatEnum;
			SelectedCard.Value = Cards[i].Value;
			OnCardSelected.Broadcast(SelectedCard);
			break;
		}
	}
}

void UASeedUI_LevelUp::InitializeCards(const TArray<FLevelUpRewardInfo>& ShuffleResults)
{
	for (int32 i=0; i<3; i++)
	{
		Cards[i].Stat->SetText(ShuffleResults[i].StatName);
		
		int32 Value = FMath::RandRange(ShuffleResults[i].MinValue, ShuffleResults[i].MaxValue);
		Cards[i].Value = Value;
		
		FString FullDesc = ShuffleResults[i].Description + FString::FromInt(Value);
		Cards[i].Desc->SetText(FText::FromString(FullDesc));

		Cards[i].StatEnum = ShuffleResults[i].StatEnum;
	}
}
