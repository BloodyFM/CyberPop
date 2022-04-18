// Fill out your copyright notice in the Description page of Project Settings.


#include "Main2AnimInstance.h"
#include "Main2.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMain2AnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UMain2AnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			Main = Cast<AMain2>(Pawn);
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

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		Main = Cast<AMain2>(Pawn);

	}
}
