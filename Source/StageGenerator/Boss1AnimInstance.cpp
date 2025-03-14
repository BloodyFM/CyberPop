// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss1AnimInstance.h"
#include "Boss1.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBoss1AnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UBoss1AnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			Boss = Cast<ABoss1>(Pawn);
		}
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		Boss = Cast<ABoss1>(Pawn);

	}
}