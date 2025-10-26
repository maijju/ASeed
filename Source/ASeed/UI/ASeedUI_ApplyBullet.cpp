// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_ApplyBullet.h"

void UASeedUI_ApplyBullet::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CurrentModuleName = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentModuleName")));
	CurrentModuleDesc = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentModuleDesc")));
	DetectedModuleName = Cast<UTextBlock>(GetWidgetFromName(TEXT("DetectedModuleName")));
	DetectedModuleDesc = Cast<UTextBlock>(GetWidgetFromName(TEXT("DetectedModuleDesc")));

	ApplyButton = Cast<UButton>(GetWidgetFromName(TEXT("ApplyButton")));
	DiscardButton = Cast<UButton>(GetWidgetFromName(TEXT("DiscardButton")));

	GameMode = Cast<AASeedGameMode>(UGameplayStatics::GetGameMode(this));
	ApplyButton->OnClicked.AddDynamic(GameMode, &AASeedGameMode::OnApplyBullet);
	DiscardButton->OnClicked.AddDynamic(GameMode, &AASeedGameMode::OnDiscardChange);

}

void UASeedUI_ApplyBullet::InitializedModules(const FModuleSummary& CurrentModule, const FModuleSummary& DetectedModule)
{
	CurrentModuleName->SetText(FText::FromName(CurrentModule.ModuleName));
	CurrentModuleDesc->SetText(FText::FromString(CurrentModule.ModuleDesc));

	DetectedModuleName->SetText(FText::FromName(DetectedModule.ModuleName));
	DetectedModuleDesc->SetText(FText::FromString(DetectedModule.ModuleDesc));
}
