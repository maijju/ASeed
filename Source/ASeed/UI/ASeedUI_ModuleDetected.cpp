// Fill out your copyright notice in the Description page of Project Settings.


#include "ASeedUI_ModuleDetected.h"

void UASeedUI_ModuleDetected::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ModuleName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ModuleName")));
	ModuleType = Cast<UTextBlock>(GetWidgetFromName(TEXT("ModuleType")));
	ModuleDesc = Cast<UTextBlock>(GetWidgetFromName(TEXT("ModuleDesc")));
	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("ConfirmButton")));

	GameMode = Cast<AASeedGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		ConfirmButton->OnClicked.AddDynamic(GameMode, &AASeedGameMode::OnConfirmModule);
	}
}

void UASeedUI_ModuleDetected::InitializeModule(const FModuleSummary& Module)
{
	ModuleName->SetText(FText::FromName(Module.ModuleName));
	if (Module.ModuleType == FName("Skill"))
		ModuleType->SetText(FText::FromString(TEXT("스킬 모듈")));
	else
		ModuleType->SetText(FText::FromString(TEXT("탄환 모듈")));
	ModuleDesc->SetText(FText::FromString(Module.ModuleDesc));
}
