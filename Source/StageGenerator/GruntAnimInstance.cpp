// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntAnimInstance.h"
#include "Grunt.h"


void UGruntAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<AGrunt>(Pawn);
		}
	}
}

void UGruntAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector2D LateralSpeed = FVector2D(Speed.X, Speed.Y);
		MovementSpeed = LateralSpeed;
		Enemy = Cast<AGrunt>(Pawn);
	}
}