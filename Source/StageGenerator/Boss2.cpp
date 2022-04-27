// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss2.h"
#include "AIController.h"
#include "AIModule.h"
#include "Kismet/KismetMathLibrary.h"


ABoss2::ABoss2()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABoss2::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

}

void ABoss2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterpToTarget && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, TurnRate);

		SetActorRotation(InterpRotation);
	}
}

FRotator ABoss2::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator YawLookAtRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
	return YawLookAtRotation;
}

void ABoss2::Aggro(ACreature* target)
{
	AIController = Cast<AAIController>(GetController());
	CombatTarget = target;
	bInterpToTarget = true;
}