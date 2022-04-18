// Fill out your copyright notice in the Description page of Project Settings.


#include "Main2.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Engine/SkeletalMeshSocket.h"

AMain2::AMain2()
{
	FistBoxL = CreateDefaultSubobject<UBoxComponent>(TEXT("FistBoxL"));
	FistBoxR = CreateDefaultSubobject<UBoxComponent>(TEXT("FistBoxR"));

	FistBoxL->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftFistSocket"));
	FistBoxR->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightFistSocket"));
}

void AMain2::BeginPlay()
{
	Super::BeginPlay();

}

void AMain2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}