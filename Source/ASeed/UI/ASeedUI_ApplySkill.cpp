// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_ApplySkill.h"

void UASeedUI_ApplySkill::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CurrentModuleNameA = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentModuleNameA")));
	CurrentModuleDescA = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentModuleDescA")));
	
	CurrentModuleNameB = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentModuleNameB")));
	CurrentModuleDescB = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentModuleDescB")));

	DetectedModuleName = Cast<UTextBlock>(GetWidgetFromName(TEXT("DetectedModuleName")));
	DetectedModuleDesc = Cast<UTextBlock>(GetWidgetFromName(TEXT("DetectedModuleDesc")));

	ApplyAButton = Cast<UButton>(GetWidgetFromName(TEXT("ApplyAButton")));
	ApplyBButton = Cast<UButton>(GetWidgetFromName(TEXT("ApplyBButton")));
	DiscardButton = Cast<UButton>(GetWidgetFromName(TEXT("DiscardButton")));

	GameMode = Cast<AASeedGameMode>(UGameplayStatics::GetGameMode(this));
	ApplyAButton->OnClicked.AddDynamic(GameMode, &AASeedGameMode::OnApplySkillA);
	ApplyBButton->OnClicked.AddDynamic(GameMode, &AASeedGameMode::OnApplySkillB);
	DiscardButton->OnClicked.AddDynamic(GameMode, &AASeedGameMode::OnDiscardChange);
}

void UASeedUI_ApplySkill::InitializedModules(const FModuleSummary& CurrentModuleA, const FModuleSummary& CurrentModuleB, const FModuleSummary& DetectedModule)
{
	CurrentModuleNameA->SetText(FText::FromName(CurrentModuleA.ModuleName));
	CurrentModuleDescA->SetText(FText::FromString(CurrentModuleA.ModuleDesc));

	CurrentModuleNameB->SetText(FText::FromName(CurrentModuleB.ModuleName));
	CurrentModuleDescB->SetText(FText::FromString(CurrentModuleB.ModuleDesc));

	DetectedModuleName->SetText(FText::FromName(DetectedModule.ModuleName));
	DetectedModuleDesc->SetText(FText::FromString(DetectedModule.ModuleDesc));
}
