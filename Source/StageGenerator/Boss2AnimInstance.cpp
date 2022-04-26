// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss2AnimInstance.h"
#include "Boss2.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBoss2AnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UBoss2AnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			Boss = Cast<ABoss2>(Pawn);
		}
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		FRotator Rotation = Pawn->GetActorRotation();

		LateralSpeed.RotateAngleAxis(Rotation.Yaw, FVector(0, 0, 1));
		XMovement = LateralSpeed.X;
		YMovement = LateralSpeed.Y;

		Boss = Cast<ABoss2>(Pawn);

	}
}