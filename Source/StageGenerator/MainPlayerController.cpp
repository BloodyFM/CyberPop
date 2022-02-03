// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (MenuOverlayAsset && LevelName == "CharSelect")
	{
		MenuOverlay = CreateWidget<UUserWidget>(this, MenuOverlayAsset);
		MenuOverlay->AddToViewport();
		MenuOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	else if (HUDOverlayAsset && LevelName != "CharSelect")
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

